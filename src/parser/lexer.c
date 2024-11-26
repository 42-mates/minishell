/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:47:23 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/26 00:56:57 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    unclosed_quotes(char *line)
{
    bool single_q;
    bool double_q;
    int i;

    i = 0;
    single_q = false;
    double_q = false;
    while (line[i])
    {
        if (line[i] == '\'' && !double_q)
            single_q = !single_q;
        else if (line[i] == '"' && !single_q)
            double_q = !double_q;
        i++;
    }
    return (single_q || double_q);
}

t_token *add_token(t_token *tokens, char *value, t_token_type type)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return NULL;
        
    new->type = type;
    new->value = value;
    new->next = NULL;

    if (!tokens)
        return new;

    t_token *temp = tokens;
    while (temp->next)
        temp = temp->next;

    temp->next = new;
    return tokens;
}

bool is_metacharacter(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token *handle_metacharacter(t_token *tokens, char *line, int *i)
{
    if (line[*i] == '|')
    {
        tokens = add_token(tokens, ft_strdup("|"), PIPE);
        (*i)++;
    }
    else if (line[*i] == '>')
    {
        if (line[*i + 1] == '>')
        {
            tokens = add_token(tokens, ft_strdup(">>"), APPEND);
            (*i) += 2;
        }
        else
        {
            tokens = add_token(tokens, ft_strdup(">"), REDIRECT_OUT);
            (*i)++;
        }
    }
    else if (line[*i] == '<')
    {
        if (line[*i + 1] == '<')
        {
            tokens = add_token(tokens, ft_strdup("<<"), HEREDOC);
            (*i) += 2;
        }
        else
        {
            tokens = add_token(tokens, ft_strdup("<"), REDIRECT_IN);
            (*i)++;
        }
    }
    return tokens;
}

t_token *lexer(char *line, t_shell *shell)
{
    t_token *tokens;
    int i = 0;
    char *value;

    tokens = NULL;
    if (unclosed_quotes(line))
    {
        ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
        shell->exit_status = 2;
        return NULL;
    }
    while (line[i])
    {
        if (ft_isspace(line[i]))
        {
            i++;
            continue;
        }
        else if (line[i] == '\'')
        {
            value = extract_single_quoted_token(line, &i);
            tokens = add_token(tokens, value, WORD);
        }
        else if (line[i] == '"')
        {
            char *value = extract_double_quoted_token(line, &i, shell->env_vars, shell);
            tokens = add_token(tokens, value, WORD);
        }
        else if (line[i] == '$')
        {
            char *value = extract_variable(line, &i, shell->env_vars, shell);
            tokens = add_token(tokens, value, WORD);
        }
        else if (is_metacharacter(line[i]))
            tokens = handle_metacharacter(tokens, line, &i);
        else
        {
            char *value = extract_word(line, &i, shell->env_vars, shell);
            tokens = add_token(tokens, value, WORD);
        }     
    }
    return (tokens);
}



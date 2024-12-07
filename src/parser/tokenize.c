/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 21:39:15 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/07 15:30:59 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*add_token(t_token *tokens, char *value, t_token_type type)
{
	t_token	*new;
	t_token	*temp;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = value;
	new->next = NULL;
	if (!tokens)
		return (new);
	temp = tokens;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (tokens);
}

t_token	*process_meta(t_token *tokens, char *line, int *i)
{
	if (line[*i] == '|')
		tokens = add_token(tokens, ft_strdup("|"), PIPE);
	else if (line[*i] == '>' && line[*i + 1] == '>')
	// TODO : Too many instructions on a single line	
		tokens = add_token(tokens, ft_strdup(">>"), APPEND), (*i)++;
	else if (line[*i] == '>')
		tokens = add_token(tokens, ft_strdup(">"), REDIRECT_OUT);
	else if (line[*i] == '<' && line[*i + 1] == '<')
	// TODO : Too many instructions on a single line
		tokens = add_token(tokens, ft_strdup("<<"), HEREDOC), (*i)++;
	else if (line[*i] == '<')
		tokens = add_token(tokens, ft_strdup("<"), REDIRECT_IN);
	(*i)++;
	return (tokens);
}

t_token	*process_word(char *line, int *i, t_shell *shell, t_token *tokens)
{
	char	*value;

	value = extract_word(line, i, shell);
	return (add_token(tokens, value, WORD));
}

t_token	*process_variable(char *line, int *i, t_shell *shell, t_token *tokens)
{
	char	*value;

	value = extract_var(line, i, shell);
	return (add_token(tokens, value, WORD));
}

t_token	*process_quotes(char *line, int *i, t_shell *shell, t_token *tokens)
{
	char	*value;

	value = NULL;
	if (line[*i] == '\'')
		value = single_quote(line, i);
	else if (line[*i] == '"')
		value = double_quote(line, i, shell);
	return (add_token(tokens, value, WORD));
}

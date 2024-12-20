/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 21:39:15 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/12 07:16:12 by oprosvir         ###   ########.fr       */
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

t_token	*meta_token(t_token *tokens, char *line, int *i)
{
	if (line[*i] == '|')
		tokens = add_token(tokens, ft_strdup("|"), PIPE);
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		tokens = add_token(tokens, ft_strdup(">>"), R_APPEND);
		(*i)++;
	}
	else if (line[*i] == '>')
		tokens = add_token(tokens, ft_strdup(">"), R_OUTPUT);
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		tokens = add_token(tokens, ft_strdup("<<"), R_HEREDOC);
		(*i)++;
	}
	else if (line[*i] == '<')
		tokens = add_token(tokens, ft_strdup("<"), R_INPUT);
	(*i)++;
	return (tokens);
}

t_token	*word_token(char *line, int *i, t_shell *shell, t_token *tokens)
{
	char	*value;

	value = extract_word(line, i, shell);
	if (value)
		tokens = add_token(tokens, value, WORD);
	return (tokens);
}

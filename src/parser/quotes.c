/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:37:36 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/26 23:40:27 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_var(char *line, int *i, t_shell *shell, char *value)
{
	char *var_value;
	char *new_value;

	var_value = extract_var(line, i, shell);
	if (!var_value)
		return (NULL);
	new_value = ft_strjoin(value, var_value);
	free(var_value);
	if (!new_value)
	{
		free(value);
		return (NULL);
	}
	return (new_value);
}

char *add_char(char *line, int *i, char *value)
{
	char *new_value;

	new_value = ft_strjoin_char(value, line[*i]);
	if (!new_value)
	{
		free(value);
		return (NULL);
	}
	(*i)++;
	return (new_value);
}

char	*double_quote(char *line, int *i, t_shell *shell)
{
	char	*value;

	++(*i);
	value = ft_strdup("");
	while (line[*i] && line[*i] != '"')
	{
		if (line[*i] == '$')
			value = expand_var(line, i, shell, value);
		else
			value = add_char(line, i, value);
		if (!value)
			return (NULL);
	}
	if (line[*i] == '"')
		(*i)++;
	return (value);
}

char	*single_quote(char *line, int *i)
{
	int start;
	char *value;

	start = ++(*i);
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	value = ft_substr(line, start, *i - start);
	if (line[*i] == '\'')
		(*i)++;
	return (value);
}

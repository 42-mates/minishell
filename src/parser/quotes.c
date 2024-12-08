/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:35:23 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/08 06:22:25 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int		start;
	char	*value;

	start = ++(*i);
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	value = ft_substr(line, start, *i - start);
	if (line[*i] == '\'')
		(*i)++;
	return (value);
}

char	*quotes_internal(char *line, int *i, void *shell)
{
	if (line[*i] == '\'')
		return (single_quote(line, i));
	else
		return (double_quote(line, i, shell));
}

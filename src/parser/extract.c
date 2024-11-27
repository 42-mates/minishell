/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 22:45:36 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/27 10:22:24 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_var(char *line, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		start;

	(*i)++;
	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (start == *i)
		return (ft_strdup("$"));
	var_name = ft_substr(line, start, *i - start);
	if (!var_name)
		return (NULL);
	var_value = getenv_lst(var_name, shell->env_vars);
	free(var_name);
	if (var_value)
		return (ft_strdup(var_value));
	else
		return (ft_strdup(""));
}

char	*expand_var(char *line, int *i, t_shell *shell, char *value)
{
	char	*var_value;
	char	*new_value;

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

char	*extract_word(char *line, int *i, t_shell *shell)
{
	char	*value;

	value = ft_strdup("");
	if (!value)
		return (NULL);
	while (line[*i] && !ft_isspace(line[*i]) && !is_meta(line[*i]))
	{
		if (line[*i] == '$')
			value = expand_var(line, i, shell, value);
		else
			value = add_char(line, i, value);
		if (!value)
			return (NULL);
	}
	return (value);
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

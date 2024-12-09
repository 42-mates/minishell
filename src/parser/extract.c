/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 22:45:36 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/09 21:21:22 by oprosvir         ###   ########.fr       */
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
	free(value);
	if (!new_value)
	{
		free(value);
		return (NULL);
	}
	return (new_value);
}

static char	*get_unquoted(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && !ft_isspace(line[*i]) && !is_meta(line[*i])
		&& line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	*extract_word(char *line, int *i, t_shell *shell)
{
	char	*value;
	char	*chunk;
	char	*tmp;

	value = ft_strdup("");
	while (line[*i] && !ft_isspace(line[*i]) && !is_meta(line[*i]))
	{
		chunk = NULL;
		if (line[*i] == '\'' || line[*i] == '"')
			chunk = quotes(line, i, shell);
		else if (line[*i] == '$')
			chunk = extract_var(line, i, shell);
		else
			chunk = get_unquoted(line, i);
		tmp = ft_strjoin(value, chunk);
		free(value);
		free(chunk);
		if (!tmp)
			return (NULL);
		value = tmp;
	}
	return (value);
}

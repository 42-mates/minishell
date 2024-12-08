/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 22:45:36 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/08 06:28:43 by oprosvir         ###   ########.fr       */
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

char	*extract_word(char *line, int *i, t_shell *shell)
{
	char	*value;
	char	*quoted;
	char	*temp;

	value = ft_strdup("");
	while (line[*i] && !ft_isspace(line[*i]) && !is_meta(line[*i]))
	{
		if (line[*i] == '$')
			value = expand_var(line, i, shell, value);
		else if (line[*i] == '\'' || line[*i] == '"')
		{
			quoted = quotes_internal(line, i, shell);
			temp = value;
			value = ft_strjoin(value, quoted);
			free(temp);
			free(quoted);
		}
		else
			value = add_char(line, i, value);
		if (!value)
			return (NULL);
	}
	return (value);
}

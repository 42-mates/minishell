/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:22:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/27 10:23:50 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	welcome_message(void)
{
	ft_putstr_fd(" __  __ _       _     _          _ _ \n", 1);
	ft_putstr_fd("|  \\/  (_)_ __ (_)___| |__   ___| | |\n", 1);
	ft_putstr_fd("| |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |\n", 1);
	ft_putstr_fd("| |  | | | | | | \\__ \\ | | |  __/ | |\n", 1);
	ft_putstr_fd("|_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|\n", 1);
	ft_putstr_fd("\n", 1);
}

bool	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*add_char(char *line, int *i, char *value)
{
	char	*new_value;

	new_value = ft_strjoin_char(value, line[*i]);
	if (!new_value)
	{
		free(value);
		return (NULL);
	}
	(*i)++;
	return (new_value);
}

char	*ft_strjoin_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

bool	is_empty_line(const char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line))
			return (false);
		line++;
	}
	return (true);
}

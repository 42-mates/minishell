/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:22:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/16 16:41:48 by oprosvir         ###   ########.fr       */
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

// note : rewrite to use
void	error_exit(char *msg)
{
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	exit(EXIT_FAILURE);
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

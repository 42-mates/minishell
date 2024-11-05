/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:22:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/05 21:28:13 by oprosvir         ###   ########.fr       */
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

void	free_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	free(cmd);
}

// for debugging
void	print_command(t_command *cmd)
{
	int	i;

	if (!cmd)
	{
		printf("Command is NULL\n");
		return ;
	}
	printf("Command name: %s\n", cmd->name);
	printf("Arguments:\n");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("  args[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
	}
	else
		printf("  No arguments\n");
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

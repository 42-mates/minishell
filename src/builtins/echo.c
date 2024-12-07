/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:12:35 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/07 15:28:51 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_options(char **args, int *newline)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		*newline = 0;
		i++;
	}
	return (i);
}

int	ft_echo(t_command *cmd)
{
	int	i;
	int	newline;

	newline = 1;
	if (cmd->args[1] == NULL)
		ft_putendl_fd("", 1);
	else
	{
		i = parse_options(cmd->args, &newline);
		while (cmd->args[i])
		{
			ft_putstr_fd(cmd->args[i], 1);
			if (cmd->args[i + 1])
				ft_putstr_fd(" ", 1);
			i++;
		}
		if (newline)
			ft_putchar_fd('\n', 1);
	}
	return (SUCCESS);
}

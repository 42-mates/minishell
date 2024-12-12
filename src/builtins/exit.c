/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:13:08 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/12 09:51:03 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	syntax_error(t_command *cmd)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(cmd->args[1], 2);
	ft_putendl_fd(": numeric argument required", 2);
	return (2);
}

static int	get_exit_code(char *arg, t_command *cmd)
{
	int		out_of_range;
	long	exit_code;

	exit_code = ft_atol(arg, &out_of_range);
	if (!out_of_range)
		return (exit_code % 256);
	return (syntax_error(cmd));
}

void	ft_exit(t_command *cmd, t_shell *shell)
{
	ft_putendl_fd(cmd->name, 1);
	if (cmd->args[1])
	{
		if (cmd->args[2])
		{
			if (is_numeric(cmd->args[1]))
			{
				ft_putendl_fd("minishell: exit: too many arguments", 2);
				shell->exit_status = 1;
				return ;
			}
			shell->exit_status = syntax_error(cmd);
		}
		else
		{
			if (is_numeric(cmd->args[1]))
				shell->exit_status = get_exit_code(cmd->args[1], cmd);
			else
				shell->exit_status = syntax_error(cmd);
		}
	}
	free_commands(cmd);
	exit(free_shell(shell));
}

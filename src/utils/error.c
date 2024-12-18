/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:06:11 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/18 13:29:07 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * print error messages and
 * return exit_status
 */
int	cmd_err(char *cmd, char *arg, char *msg, int err_num)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
	return (err_num);
}

void	*set_status(t_shell *shell, int status)
{
	shell->exit_status = status;
	return (NULL);
}

/**
 * is used to return from func. with NULL while setting exit_status
 * and writing error message
 */
void	*err_msg(char *cmd, char *msg, t_shell *shell, int exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
	return (set_status(shell, exit_status));
}

int	exec_error(char *cmd)
{
	if (errno == EACCES)
		return (cmd_err(cmd, NULL, "Permission denied", 126));
	else if (errno == ENOENT)
		return (cmd_err(cmd, NULL, "No such file or directory", 127));
	else if (errno == ENOTDIR)
		return (cmd_err(cmd, NULL, "Not a directory", 127));
	else
		return (cmd_err(cmd, NULL, strerror(errno), 1));
}

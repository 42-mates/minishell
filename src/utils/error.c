/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:06:11 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/11 23:29:06 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * print error messages and
 * return exit_status in builtins
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

void	exec_error(char *cmd, t_shell *shell)
{
	if (errno == EACCES) // Permission denied
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Permission denied", 2);
		shell->exit_status = 126;
	}
	else if (errno == ENOENT) // File not found
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		shell->exit_status = 127;
	}
	else if (errno == ENOTDIR) // Not a directory
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Not a directory", 2); 
		shell->exit_status = 127;
	}
	// else if (errno == EISDIR) // Is a directory
	// {
	// 	ft_putstr_fd("minishell: ", 2);
	// 	ft_putstr_fd(cmd, 2);
	// 	ft_putendl_fd(": Is a directory", 2);
	// 	shell->exit_status = 126;
	// }	
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		shell->exit_status = 1; // General failure
	}
}

// TODO : rewrite to use
// void	error_exit(char *msg)
// {
// 	ft_putstr_fd(strerror(errno), 2);
// 	ft_putstr_fd(": ", 2);
// 	ft_putendl_fd(msg, 2);
// 	exit(EXIT_FAILURE);
// }

// void	errmsg_cmd(char *cmd, char *arg, char *error_msg)
// {
// 	ft_putstr_fd("minishell: ", 2);
// 	ft_putstr_fd(cmd, 2);
// 	ft_putstr_fd(": ", 2);
// 	if (arg)
// 	{
// 		ft_putstr_fd(arg, 2);
// 		ft_putstr_fd(": ", 2);
// 	}
// 	ft_putendl_fd(error_msg, 2);
// }

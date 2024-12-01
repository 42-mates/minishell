/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:06:11 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/28 20:40:00 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *set_status(t_shell *shell, int status)
{
    shell->exit_status = status;
    return (NULL);
}

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

// TODO : rewrite to use
void	error_exit(char *msg)
{
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	exit(EXIT_FAILURE);
}

/*
void handle_exec_error(const char *cmd, t_shell *shell)
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
    else
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd, 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd(strerror(errno), 2);
        shell->exit_status = 1; // General failure
    }
}

void executor(t_command *cmd, t_shell *shell)
{
    if (execvp(cmd->name, cmd->args) == -1)
    {
        handle_exec_error(cmd->name, shell);
        exit(shell->exit_status);
    }
}

*/

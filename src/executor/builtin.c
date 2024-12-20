/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:00:57 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/20 09:36:58 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	backup_fds(int *fds, t_shell *shell)
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] < 0 || fds[1] < 0)
	{
		perror("dup");
		shell->exit_status = 1;
		return (-1);
	}
	return (0);
}

static void	restore_fds(int *fds)
{
	if (!fds)
		return ;
	if (fds[0] >= 0)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	if (fds[1] >= 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
}

int	is_builtin(const char *cmd_name)
{
	if (!cmd_name)
		return (0);
	return (ft_strcmp(cmd_name, "cd") == 0 || ft_strcmp(cmd_name, "echo") == 0
		|| ft_strcmp(cmd_name, "pwd") == 0 || ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0 || ft_strcmp(cmd_name, "env") == 0
		|| ft_strcmp(cmd_name, "exit") == 0);
}

void	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->name, "exit") == 0)
		ft_exit(cmd, shell);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		shell->exit_status = ft_pwd(cmd, shell);
	else if (ft_strcmp(cmd->name, "echo") == 0)
		shell->exit_status = ft_echo(cmd);
	else if (ft_strcmp(cmd->name, "env") == 0)
		shell->exit_status = ft_env(cmd, shell);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		shell->exit_status = ft_unset(cmd, shell);
	else if (ft_strcmp(cmd->name, "export") == 0)
		shell->exit_status = ft_export(cmd, shell);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		shell->exit_status = ft_cd(cmd, shell);
}

void	case_builtin(t_command *cmd, t_shell *shell)
{
	int	original_fds[2];

	original_fds[0] = -1;
	original_fds[1] = -1;
	if (cmd->redirects)
	{
		if (backup_fds(original_fds, shell) == -1)
			return ;
		if (cmd_heredocs(cmd) == -1)
		{
			shell->exit_status = 1;
			return ;
		}
		if (set_redirection(cmd) == -1)
		{
			restore_fds(original_fds);
			shell->exit_status = 1;
			return ;
		}
	}
	execute_builtin(cmd, shell);
	if (cmd->redirects)
		restore_fds(original_fds);
}

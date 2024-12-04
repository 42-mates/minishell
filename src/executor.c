/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/04 18:49:48 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(const char *cmd_name)
{
    return (ft_strcmp(cmd_name, "cd") == 0 ||
            ft_strcmp(cmd_name, "echo") == 0 ||
            ft_strcmp(cmd_name, "pwd") == 0 ||
            ft_strcmp(cmd_name, "export") == 0 ||
            ft_strcmp(cmd_name, "unset") == 0 ||
            ft_strcmp(cmd_name, "env") == 0 ||
            ft_strcmp(cmd_name, "exit") == 0);
}

void	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->name, "exit") == 0)
		ft_exit(cmd, shell);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(cmd->name, "echo") == 0)
		ft_echo(cmd, shell);
	else if (ft_strcmp(cmd->name, "env") == 0)
		ft_env(cmd, shell);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		shell->exit_status = ft_unset(cmd, shell);
	else if (ft_strcmp(cmd->name, "export") == 0)
		shell->exit_status = ft_export(cmd, shell);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		shell->exit_status = ft_cd(cmd, shell);
}

void	execute_command(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**envp;

	envp = convert_to_array(shell->env_vars);
	if (!envp)
	{
		shell->exit_status = 1;
		return ;
	}
	// int		pipefd[2];
	// int		wstatus;
	// if (pipe(pipefd) == -1)
	// {
	// 	perror("pipe");
	// 	shell->exit_status = errno;
	// 	return ;
	// }
	pid = fork();
	if (pid < 0)
	{
		// close(pipefd[0]);
		// close(pipefd[1]);
		perror("fork");
		shell->exit_status = errno;
		ft_putendl_fd(strerror(errno), 2);
		return ;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(cmd->args[0], cmd->args, envp) == -1)
		{
			perror("execvp");
			free_memory(envp);
			exit(126);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		shell->exit_status = 1;
	}
	else
	{
		status = 0;
		waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		else
			shell->exit_status = 1;
	}
	free_memory(envp);
}

void	executor(t_command *cmd, t_shell *shell)
{
	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
		execute_command(cmd, shell);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/14 20:11:26 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// note : заглушка для начала работы executor + pwd, echo
static int	is_builtin(const char *cmd_name)
{
	const char	*builtins[] = {"cd", "export", "unset", "env", "exit", NULL};

	for (int i = 0; builtins[i]; i++)
	{
		if (ft_strcmp(cmd_name, builtins[i]) == 0)
			return (1);
	}
	return (0);
}

void	execute_command(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*path;

	envp = convert_to_array(shell->env_vars);
	if (!envp)
	{
		shell->exit_status = 1;
		return ;
	}
	path = find_relative_path(cmd->name, envp);
	if (!path)
	{
		ft_putstr_fd(cmd->name, 2);
		ft_putstr_fd(": command not found\n", 2);
		shell->exit_status = 127;
		free_memory(envp);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(path, cmd->args, envp) == -1)
		{
			perror("execvp");
			free(path);
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
	free(path);
	free_memory(envp);
}

void	executor(t_command *cmd, t_shell *shell)
{
	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
		execute_command(cmd, shell);
}

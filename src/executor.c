/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/13 21:35:57 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// note : заглушка для начала работы executor + pwd, echo
static int	is_builtin(const char *cmd_name)
{
    const char *builtins[] = {"cd", "export", "unset", "env", "exit", NULL};
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

	pid = fork();
	envp = convert_env_list_to_array(shell->env_vars);
	path = find_relative_path(cmd->name, envp);
	if (pid == 0)
	{
		if (execve(path, cmd->args, envp) == -1)
			perror("execvp");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		}
		while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

void	executor(t_command *cmd, t_shell *shell)
{
	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
		execute_command(cmd, shell);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/11 19:48:59 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// note : заглушка для начала работы executor + pwd, echo
static int	is_builtin(const char *cmd_name)
{
    const char *builtins[] = {"cd", "export", "unset", "env", "exit", "pwd", "echo", NULL};
    for (int i = 0; builtins[i]; i++)
	{
		if (ft_strcmp(cmd_name, builtins[i]) == 0)
			return (1);
	}
	return (0);
}

// note : нужна реализация
void	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->name, "exit") == 0)
		ft_exit(cmd, shell);
	if (ft_strcmp(cmd->name, "pwd") == 0)
		ft_pwd(shell);
	if (ft_strcmp(cmd->name, "echo") == 0)
		ft_echo(cmd, shell);
}

char	*ft_getenv(const char *name, char **envp)
{
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, ft_strlen(name)) == 0)
		{
			value = envp[i] + ft_strlen(name) + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

void	free_memory(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void 	find_relative_path(t_command *cmd, char **envp)
{
	char	*path;
	char	**split_paths;
	char	*path_slash;
	int		i;

	split_paths = ft_split(ft_getenv("PATH", envp), ':');
	i = 0;
	while (split_paths[i])
	{
		path_slash = ft_strjoin(split_paths[i], "/");
		path = ft_strjoin(path_slash, cmd->path);
		free(path_slash);
		if (access(path, X_OK) == 0)
		{
			cmd->path = path;
			free(path);
			return;
		}
		free(path);
		i++;
	}
	free_memory(split_paths);
}

void 	check_absolute_path(t_command *cmd, char **envp)
{
	if (cmd->args[0][0] == '/')
	{
		if (access(cmd->args[0], X_OK) == 0)
			cmd->path = cmd->args[0];
		else
		{
			// display_error(cmd->args[0]);
			perror(cmd->args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
		find_relative_path(cmd, envp);
}


void	execute_command(t_command *cmd, t_shell *shell, char **envp)
{
	pid_t	pid;
	// int		pipefd[2];
	int		wstatus;

	// if (pipe(pipefd) == -1)
	// {
	// 	perror("pipe");
	// 	shell->exit_status = errno;
	// 	return;
	// }
	pid = fork();
	if (pid < 0)
	{
		// close(pipefd[0]);
		// close(pipefd[1]);
		perror("fork");
		shell->exit_status = errno;
		ft_putendl_fd(strerror(errno), 2);
		return;
	}
	if (pid == 0)
	{
		if (execve(cmd->path, cmd->args, envp) == -1)
		{
			ft_putendl_fd(strerror(errno), 2);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		do
			waitpid(pid, &wstatus, WUNTRACED);
		while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	}
}

void	executor(t_command *cmd, t_shell *shell)
{
	char	**envp;

	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
	{
		envp = convert_env_list_to_array(shell->env_vars);
		check_absolute_path(cmd, envp);
		execute_command(cmd, shell, envp);
	}
}

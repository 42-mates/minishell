/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/02 18:44:28 by mglikenf         ###   ########.fr       */
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
		ft_unset(cmd, shell);
	else if (ft_strcmp(cmd->name, "export") == 0)
		ft_export(cmd, shell);
}

int	count_cmds(t_command *cmd)
{
	t_command	*current;
	int			n_cmds;

	n_cmds = 0;
	current = cmd;
	while (current)
	{
		n_cmds++;
		current = current->next;
	}
	return (n_cmds);
}

void	display_error_and_return(char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_putendl_fd(strerror(errno), 2);
	return ;
}

void	create_pipes(t_pipe *pipeline)
{
	int	i;

	i = 0;
	while (i <= pipeline->n_pipes)
	{
		if (pipe(pipeline->pipefd[i]) == -1 && i < pipeline->n_pipes)
		{
			while (i > 0)
			{
				close(pipeline->pipefd[i - 1][0]);
				close(pipeline->pipefd[i - 1][1]);
				i--;
			}
			display_error_and_return("minishell: pipe: ");
			exit (EXIT_FAILURE);
		}
		i++;
	}
}

void	duplicate_fds(t_pipe *pipeline, int i)
{
	if (i > 0)
	{
		if (dup2(pipeline->pipefd[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 read");
			exit(EXIT_FAILURE);
		}
		close(pipeline->pipefd[i - 1][0]);
	}
	if (i < pipeline->n_pipes)
	{
		if (dup2(pipeline->pipefd[i][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 write");
			exit(EXIT_FAILURE);
		}
		close(pipeline->pipefd[i][1]);
	}
}

void	child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
{
	char	**envp;

	envp = convert_to_array(shell->env_vars);
	if (!envp)
	{
		shell->exit_status = 1;
		return ;
		// exit(EXIT_FAILURE); --??
	}
	duplicate_fds(pipeline, i);
	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
	{
		if (execve(cmd->name, cmd->args, envp) == -1)
		{
			free_memory(envp);
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
}

void	parent_process(t_pipe *pipeline, pid_t pids[MAX_PIPES + 1])
{
	int	i;
	int	status;

	i = 0;
	while (i <= pipeline->n_pipes)
	{
		close(pipeline->pipefd[i][0]);
		close(pipeline->pipefd[i][1]);
		i++;
	}
	i = 0;
	while (i <= pipeline->n_pipes)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}

void	execute_extern(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	t_command	*current_cmd;
	pid_t		pids[MAX_PIPES + 1];
	int			i;
	
	current_cmd = cmd;
	create_pipes(pipeline);
	i = 0;
	while (i <= pipeline->n_pipes)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			child_process(current_cmd, shell, pipeline, i);
		else if (pids[i] < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (i > 0)
			close(pipeline->pipefd[i - 1][0]);
		if (i < pipeline->n_pipes)
			close(pipeline->pipefd[i][1]);
		current_cmd = current_cmd->next;
		i++;
	}
	parent_process(pipeline, pids);
}

void	executor(t_command *cmd, t_shell *shell)
{
	t_pipe	*pipeline;
	int		n_cmds;

	pipeline = malloc(sizeof(t_pipe));
	if (!pipeline)
	{
		perror("malloc pipeline");
		return ;
	}
	ft_memset(pipeline, 0, sizeof(t_pipe));
	n_cmds = count_cmds(cmd);
	pipeline->n_pipes = n_cmds - 1;
	if (pipeline->n_pipes > MAX_PIPES)
	{
		ft_putendl_fd("pipe limit exceeded", 2);
		free(pipeline);
		return ;
	}
	if (is_builtin(cmd->name) && n_cmds < 2)
		execute_builtin(cmd, shell);
	else
		execute_extern(cmd, shell, pipeline);
	free(pipeline);
}

// void	execute_command(t_command *cmd, t_shell *shell)
// {
// 	pid_t	pid;
// 	int		status;
// 	char	**envp;

// 	envp = convert_to_array(shell->env_vars);
// 	if (!envp)
// 	{
// 		shell->exit_status = 1;
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("fork");
// 		shell->exit_status = errno;
// 		ft_putendl_fd(strerror(errno), 2);
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_DFL);
// 		if (execve(cmd->args[0], cmd->args, envp) == -1)
// 		{
// 			perror("execvp");
// 			free_memory(envp);
// 			exit(126);
// 		}
// 	}
// 	else if (pid < 0)
// 	{
// 		perror("fork");
// 		shell->exit_status = 1;
// 	}
// 	else
// 	{
// 		status = 0;
// 		waitpid(pid, &status, WUNTRACED);
// 		if (WIFEXITED(status))
// 			shell->exit_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 			shell->exit_status = 128 + WTERMSIG(status);
// 		else
// 			shell->exit_status = 1;
// 	}
// 	free_memory(envp);
// }

// void	executor(t_command *cmd, t_shell *shell)
// {
// 	if (is_builtin(cmd->name))
// 		execute_builtin(cmd, shell);
// 	else
// 		execute_command(cmd, shell);
// }

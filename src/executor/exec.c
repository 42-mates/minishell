/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/17 23:24:35 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * 1. convert env variables list to an array of strings (for execve)
 * 2. if command name doesn't contain a '/', search for it in the PATH
 * 3. check if the resolved path is a directory using stat
 * 4. execute the command using execve
 * 5. exit with the appropriate error code if execve fails
 */
void	execute_extern(t_command *cmd, t_shell *shell)
{
	char	**envp;
	char	*exec_path;
	struct	stat st;
	
	envp = list_to_array(shell->env_vars);
	if (!envp)
		exit(cmd_err("malloc", NULL, "env conversion failed", 1));
	if (!cmd->name || cmd->name[0] == '\0')
		exit(SUCCESS);
	if (!ft_strchr(cmd->name, '/'))
	{
		exec_path = get_path(cmd->name, shell->env_vars);
		if (!exec_path)
			exit(cmd_err(cmd->name, NULL, "command not found", 127));
		free(cmd->args[0]);
		cmd->args[0] = exec_path;
	}
	if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
		exit(cmd_err(cmd->args[0], NULL, "Is a directory", 126));
	execve(cmd->args[0], cmd->args, envp);
	exit(exec_error(cmd->name));
}

void close_pipes(t_command *cmds)
{
	t_command *cmd = cmds;

	while (cmd)
	{
		if (cmd->pipe_in != -1)
			close(cmd->pipe_in);
		if (cmd->pipe_out != -1)
			close(cmd->pipe_out);
		cmd = cmd->next;
	}
}

void	parent_process(t_command *cmds, pid_t last_pid, t_shell *shell)
{
	pid_t pid;
	int			status;

	close_pipes(cmds);
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
	}
}

int create_pipes(t_command *cmds, t_shell *shell)
{
	t_command *cmd;
	int pipefd[2];

	cmd = cmds;
	while (cmd && cmd->next) // Создаём пайпы, пока есть следующая команда
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe: ");
			shell->exit_status = 1;
			return (-1);
		}
		cmd->pipe_out = pipefd[1];      // Текущая команда пишет в pipe_out
		cmd->next->pipe_in = pipefd[0]; // Следующая команда читает из pipe_in
		cmd = cmd->next;
	}
	return (0);
}

void redirect_pipes(t_command *cmd)
{
	if (cmd->pipe_in != -1) // Перенаправление STDIN
	{
		if (dup2(cmd->pipe_in, STDIN_FILENO) == -1)
		{
			perror("dup2: failed to redirect pipe_in");
			close(cmd->pipe_in);
			exit(EXIT_FAILURE);
		}
		close(cmd->pipe_in); // Закрываем после перенаправления
	}
	if (cmd->pipe_out != -1) // Перенаправление STDOUT
	{
		if (dup2(cmd->pipe_out, STDOUT_FILENO) == -1)
		{
			perror("dup2: failed to redirect pipe_out");
			close(cmd->pipe_out);
			exit(EXIT_FAILURE);
		}
		close(cmd->pipe_out); // Закрываем после перенаправления
	}
}

void	child_process(t_command *cmd, t_shell *shell)
{
	redirect_pipes(cmd);
	if (handle_heredocs(cmd) == -1)
		exit(EXIT_FAILURE);
	if (set_redirection(cmd) == -1)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->name))
	{
		execute_builtin(cmd, shell);
		exit(shell->exit_status);
	}
	else
		execute_extern(cmd, shell);
}

void	executor(t_command *cmds, t_shell *shell)
{
	t_command	*cmd;
	pid_t		pid;
	pid_t last_pid = 0;

	if (create_pipes(cmds, shell) == -1)
		return ;
	signal(SIGQUIT, exec_signals);
	signal(SIGINT, exec_signals);
	cmd = cmds;
	while (cmd)
	{
		pid = fork();
		if (pid < 0)
		{
			err_msg(cmd->name, "failed to create child process", shell, 1);
			return ;
		}
		else if (pid == 0)
			child_process(cmd, shell);
		else
		{
			if (cmd->pipe_out != -1)
            {
                close(cmd->pipe_out);
                cmd->pipe_out = -1;
            }
			if (cmd->pipe_in != -1)
			{
				close(cmd->pipe_in);
				cmd->pipe_in = -1;
			}
		}
		last_pid = pid;
		cmd = cmd->next;
	}
	parent_process(cmds, last_pid, shell);
}

// void	parent_process(t_shell *shell)
// {
// 	int	i;
// 	int	status;

// 	close_pipes(pipeline);
// 	i = 0;
// 	while (i <= pipeline->n_pipes)
// 	{
// 		waitpid(pids[i], &status, 0);
// 		if (WIFEXITED(status))
// 			shell->exit_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 			shell->exit_status = 128 + WTERMSIG(status);
// 		i++;
// 	}
// }

// void	child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
// {
// 	duplicate_fds(pipeline, i);
// 	for (int j = 0; j < pipeline->n_pipes; j++)
// 	{
// 		close(pipeline->pipefd[j][0]);
// 		close(pipeline->pipefd[j][1]);
// 	}
// 	if (handle_heredocs(cmd) == -1)
// 		exit(EXIT_FAILURE);
// 	if (set_redirection(cmd) == -1)
// 		exit(EXIT_FAILURE);
// 	if (is_builtin(cmd->name))
// 	{
// 		execute_builtin(cmd, shell);
// 		exit(shell->exit_status);
// 	}
// 	else
// 		execute_extern(cmd, shell);
// }

// void	execute_multi(t_command *cmd, t_shell *shell, t_pipe *pipeline)
// {
// 	t_command	*current_cmd;
// 	pid_t		pids[MAX_PIPES + 1];
// 	int			i;
	
// 	i = 0;
// 	current_cmd = cmd;
// 	if (create_pipes(pipeline, shell) == -1)
// 		return ;
// 	while (current_cmd)
// 	{
// 		pids[i] = fork();
// 		if (pids[i] < 0)
// 		{
// 			err_msg(cmd->name, "failed to create child process", shell, 1);
// 			return ;
// 		}
// 		else if (pids[i] == 0)
// 			child_process(current_cmd, shell, pipeline, i);
// 		current_cmd = current_cmd->next;
// 		i++;
// 	}
// 	parent_process(pipeline, pids, shell);
// }

// void	executor(t_command *cmd, t_shell *shell, t_pipe *pipeline)
// {
// 	signal(SIGQUIT, exec_signals);
// 	signal(SIGINT, exec_signals);
// 	execute_multi(cmd, shell, pipeline);
// }

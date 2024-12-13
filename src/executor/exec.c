/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/13 17:08:31 by oprosvir         ###   ########.fr       */
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

void	child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
{
	duplicate_fds(pipeline, i);
	for (int j = 0; j < pipeline->n_pipes; j++)
	{
		close(pipeline->pipefd[j][0]);
		close(pipeline->pipefd[j][1]);
	}
	if (cmd->delimiter)
		heredoc(cmd, shell);
	if (set_redirection(cmd, shell) == -1)
	{
		shell->exit_status = 1;
		exit(shell->exit_status);
	}
	if (is_builtin(cmd->name))
	{
		execute_builtin(cmd, shell);
		exit(shell->exit_status);
	}
	else
		execute_extern(cmd, shell);
}

void	parent_process(t_pipe *pipeline, pid_t pids[MAX_PIPES + 1], t_shell *shell)
{
	int	i;
	int	status;

	close_pipes(pipeline);
	i = 0;
	while (i <= pipeline->n_pipes)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		i++;
	}
}

void	execute_multi(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	t_command	*current_cmd;
	pid_t		pids[MAX_PIPES + 1];
	int			i;
	
	i = 0;
	current_cmd = cmd;
	if (create_pipes(pipeline, shell) == -1)
		return ;
	while (current_cmd)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			err_msg(cmd->name, "failed to create child process", shell, 1);
			return ;
		}
		else if (pids[i] == 0)
			child_process(current_cmd, shell, pipeline, i);
		if (i > 0)
		{
			close(pipeline->pipefd[i - 1][0]);
			close(pipeline->pipefd[i - 1][1]);
		}
		//close_pipe_ends(i, pipeline, current_cmd);
		current_cmd = current_cmd->next;
		i++;
	}
	parent_process(pipeline, pids, shell);
}

void	executor(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	int	original_fds[2];

	signal(SIGQUIT, exec_signals);
	signal(SIGINT, exec_signals);
	if (count_cmds(cmd) > 1)
	{
		execute_multi(cmd, shell, pipeline);
		return ;
	}
	if (!cmd->name)
	{
		if (set_redirection(cmd, shell) == -1)
		{
			shell->exit_status = 1;
			return;
		}
		return ;
	}
	if (is_builtin(cmd->name))
	{
		backup_original_fds(original_fds, shell);
		if (set_redirection(cmd, shell) == -1)
		{
			restore_original_fds(original_fds);
			shell->exit_status = 1;
			return ;
		}
		execute_builtin(cmd, shell);
		restore_original_fds(original_fds);
		return ;
	}
	execute_multi(cmd, shell, pipeline);
}

// void	executor(t_command *cmd, t_shell *shell, t_pipe *pipeline)
// {
// 	int	original_in;
// 	int	original_out;

// 	if (is_builtin(cmd->name) && count_cmds(cmd) == 1)
// 	{
// 		original_in = dup(STDIN_FILENO);
// 		original_out = dup(STDOUT_FILENO);
// 		if (original_in == -1 || original_out == -1)
// 		{
// 			perror("dup");
// 			shell->exit_status = 1;
// 		}
// 		if (cmd->input_file || cmd->output_file || cmd->append_file)
// 			set_redirection(cmd, shell);
// 		execute_builtin(cmd, shell);
// 		dup2(original_in, STDIN_FILENO);
// 		dup2(original_out, STDOUT_FILENO);
// 	}
// 	else
// 		execute_multi(cmd, shell, pipeline);
// 	free(pipeline);
// }

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

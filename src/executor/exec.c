/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/13 12:47:40 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_extern(t_command *cmd, t_pipe *pipeline, char **envp, t_shell *shell)
{
	if (execve(cmd->name, cmd->args, envp) == -1) // если успешно, новая программа (например /bin/ls) сама вызывает
	{											//   exit(code) внутри себя, и этот code получает род-ль через waitpid()	
		free_array(envp);
		close_pipes(pipeline);
		free(pipeline);
		perror("minishell: execve:");
		//exit(shell->exit_status);
		shell->exit_status = errno; // излишне, будет установлен в род.процессе
		// exit(errno); 				//	при ошибке вызова execve выходим из ПРОЦЕССА через EXIT
		// return ; 					а не из ФУНКЦИИ через return
		// errno тоже нужно обработать
		// пример: errno EACCES =-13 (126 exit status в bash)
	}
}

void	child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
{
	char	**envp;

	signal(SIGQUIT, child_signals);
	signal(SIGINT, child_signals);
	envp = list_to_array(shell->env_vars);
	if (!envp)
	{
		close_pipes(pipeline);
		free(pipeline);
		perror("minishell: failed to convert environment variables");
		exit(EXIT_FAILURE);
	}
	duplicate_fds(pipeline, i);
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
		free_array(envp);
		exit(shell->exit_status);
	}
	else
		execute_extern(cmd, pipeline, envp, shell);
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
		{
			shell->exit_status = 128 + WTERMSIG(status);
			// if (WTERMSIG(status) == SIGQUIT)
			// 	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			// if (WTERMSIG(status) == SIGINT)
			// 	write(STDERR_FILENO, "\n", 1);
		}
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
		close_pipe_ends(i, pipeline, current_cmd);
		current_cmd = current_cmd->next;
		i++;
	}
	parent_process(pipeline, pids, shell);
}

void	executor(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	int	original_fds[2];

	if (count_cmds(cmd) > 1)
		execute_multi(cmd, shell, pipeline);
	else
	{
		if (!cmd->name)
		{
			if (set_redirection(cmd, shell) == -1)
				return;
		}
		else if (cmd->name && is_builtin(cmd->name))
		{
			backup_original_fds(original_fds, shell, pipeline);
			if (set_redirection(cmd, shell) == -1)
				exit(1);
			execute_builtin(cmd, shell);
			restore_original_fds(original_fds);
		}
		else
			execute_multi(cmd, shell, pipeline);
	}
	free(pipeline);
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

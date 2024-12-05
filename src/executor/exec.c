/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/05 11:58:54 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	display_error_and_return(char *msg)
// {
// 	ft_putstr_fd(msg, 2);
// 	ft_putendl_fd(strerror(errno), 2);
// 	return ;
// }

void	duplicate_fds(t_pipe *pipeline, int i)
{
	if (i > 0)
	{
		if (dup2(pipeline->pipefd[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2: ");
			close_pipes(pipeline);
			free(pipeline);
			exit(EXIT_FAILURE);
		}
		close(pipeline->pipefd[i - 1][0]);
	}
	if (i < pipeline->n_pipes)
	{
		if (dup2(pipeline->pipefd[i][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2:");
			close_pipes(pipeline);
			free(pipeline);
			exit(EXIT_FAILURE);
		}
		close(pipeline->pipefd[i][1]);
	}
}

void	execute_extern(t_command *cmd, t_pipe *pipeline, char **envp)
{
	if (execve(cmd->name, cmd->args, envp) == -1) // если успешно, новая программа (например /bin/ls) сама вызывает
	{											//   exit(code) внутри себя, и этот code получает род-ль через waitpid()	
		free_memory(envp);
		close_pipes(pipeline);
		free(pipeline);
		perror("minishell: execve:");
		// shell->exit_status = errno; // излишне, будет установлен в род.процессе
		exit(errno); 				//	при ошибке вызова execve выходим из ПРОЦЕССА через EXIT
		// return ; 					а не из ФУНКЦИИ через return
		// errno тоже нужно обработать
		// пример: errno EACCES =-13 (126 exit status в bash)
	}
}

void	child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
{
	char	**envp;

	envp = convert_to_array(shell->env_vars);
	if (!envp)
	{
		close_pipes(pipeline);
		free(pipeline);
		perror("minishell: failed to convert environment variables");
		exit(EXIT_FAILURE);
		// return ;
		// return НЕ завершает детский процесс, вызванный через fork() 
		// return завершает только текущую функцию
	}
	duplicate_fds(pipeline, i);
	if (cmd->input_file || cmd->output_file || cmd->append_file)
		set_redirection(cmd, shell);
	if (is_builtin(cmd->name))
	{
		execute_builtin(cmd, shell);
		free_memory(envp);
		exit(shell->exit_status);
	}
	else
		execute_extern(cmd, pipeline, envp);
	// после вызова execute_extern, процесс завершится внутри функции execute_extern
	// free_memory(envp); эта строка никогда НЕ будет достигнута
}

void	parent_process(t_pipe *pipeline, pid_t pids[MAX_PIPES + 1], t_shell *shell)
{
	int	i;
	int	status;

	close_pipes(pipeline);
	i = 0;
	while (i <= pipeline->n_pipes)
	{
		waitpid(pids[i], &status, 0); // waitpid ждет ВЫХОДА из детского процесса
		if (WIFEXITED(status)) // получить код выхода доч. процесса
			shell->exit_status = WEXITSTATUS(status);
		i++;
	}
}

void	execute_multi(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	t_command	*current_cmd;
	pid_t		pids[MAX_PIPES + 1];
	int			i;
	
	current_cmd = cmd;
	if (create_pipes(pipeline, shell) == -1)
		return ;
	i = 0;
	while (current_cmd)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("minishell: fork: ");
			close_pipes(pipeline);
			shell->exit_status = 1;
			return ;
		}
		else if (pids[i] == 0)
			child_process(current_cmd, shell, pipeline, i);
		if (current_cmd->next)
			close(pipeline->pipefd[i][1]);
		if (i > 0)
			close(pipeline->pipefd[i - 1][0]);
		current_cmd = current_cmd->next;
		i++;
	}
	parent_process(pipeline, pids, shell); // передаем shell, чтобы сохранить код выхода посл. комманды
}

void	executor(t_command *cmd, t_shell *shell)
{
	t_pipe	*pipeline;

	pipeline = set_pipeline(shell, cmd);
	if (!pipeline)
		return ;
	if (pipeline->n_pipes > MAX_PIPES)
	{
		ft_putendl_fd("too many commands, pipe limit exceeded", 2);
		free(pipeline);
		return ;
	}
	if (is_builtin(cmd->name) && count_cmds(cmd) == 1)
	{
		int	original_in = dup(STDIN_FILENO);
		int	original_out = dup(STDOUT_FILENO);
		if (original_in == -1 || original_out == -1)
		{
			perror("dup");
			exit(EXIT_FAILURE);
		}
		if (cmd->input_file || cmd->output_file || cmd->append_file)
			set_redirection(cmd, shell);
		execute_builtin(cmd, shell);
		dup2(original_in, STDIN_FILENO);
		dup2(original_out, STDOUT_FILENO);
	}
	else
		execute_multi(cmd, shell, pipeline);
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

// ###########################################################

	// if (cmd->input_file && i == 0)
	// {
	// 	int fd_input = open(cmd->input_file, O_RDONLY, 0777);
	// 	if (fd_input == -1)
	// 	{
	// 		perror("failed openning input file");
	// 		free_memory(envp);
	// 	}
	// 	dup2(fd_input, pipeline->pipefd[i][0]);
	// 	close(fd_input);
	// }
	// if (cmd->output_file != NULL && i == pipeline->n_pipes)
	// {
	// 	int fd_output = open(cmd->output_file, O_WRONLY)
	// }
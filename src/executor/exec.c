/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/11 23:41:21 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <sys/types.h>

void	execute_extern(t_command *cmd, t_shell *shell)
{
	char	**envp;
	char	*exec_path;
	struct	stat st;
	
	envp = list_to_array(shell->env_vars);
	if (!envp)
	{
		perror("minishell: failed to convert environment variables");
		exit(EXIT_FAILURE);
	}
	if (!ft_strchr(cmd->name, '/'))
	{
		exec_path = get_path(cmd->name, shell->env_vars);
		if (!exec_path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->name, 2);
            ft_putendl_fd(": command not found", 2);
            free_array(envp);
            exit(127);
        }
        // free(cmd->name);
        // cmd->name = exec_path;
        free(cmd->args[0]);
        cmd->args[0] = ft_strdup(exec_path);
	}
	if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putendl_fd(": Is a directory", 2);
        free_array(envp);
        exit(126);
    }
	if (execve(cmd->args[0], cmd->args, envp) == -1)
	{	
		free_array(envp);
		exec_error(cmd->name, shell);
		exit(shell->exit_status);
	}
}

void	child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
{
	duplicate_fds(pipeline, i);
	if (cmd->delimiter)
		heredoc(cmd, shell);
	if (set_redirection(cmd, shell) == -1)
	{
		// printf("failed to open file %s\n", cmd->append_file);
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
		close_pipe_ends(i, pipeline, current_cmd);
		current_cmd = current_cmd->next;
		i++;
	}
	// if (cmd->tmp_file_path)
		// unlink(cmd->tmp_file_path);
	parent_process(pipeline, pids, shell);
}

void	executor(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	int	original_fds[2];

	signal(SIGQUIT, child_signals);
	signal(SIGINT, child_signals);
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
			backup_original_fds(original_fds, shell);
			if (set_redirection(cmd, shell) == -1)
				exit(1);
			execute_builtin(cmd, shell);
			restore_original_fds(original_fds);
		}
		else
			execute_multi(cmd, shell, pipeline);
	}
	//free(pipeline);
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

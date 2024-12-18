/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/18 00:51:10 by oprosvir         ###   ########.fr       */
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

void	child_process(t_command *cmd, t_shell *shell, int i)
{
	duplicate_fds(&shell->pipeline, i);
	close_pipes(&shell->pipeline);
	// if (handle_heredocs(cmd) == -1)
	// 	exit(EXIT_FAILURE);
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

void	parent_process(t_pipe *pipeline, pid_t *pids, t_shell *shell)
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

int all_heredocs(t_command *cmds, t_shell *shell)
{
	t_command *cur;

	cur = cmds;
	while (cur)
	{
		if (handle_heredocs(cur) == -1)
		{
			shell->exit_status = 1;
			return -1;
		}
		cur = cur->next;
	}
	return 0;
}

void	executor(t_command *cmd, t_shell *shell, t_pipe *pipeline)
{
	t_command	*cur;
	pid_t		pids[MAX_PIPES + 1];
	int			i;

	int original_stdin = dup(STDIN_FILENO);
	if (all_heredocs(cmd, shell) == -1)
		return;
	
	i = 0;
	cur = cmd;
	signal(SIGQUIT, exec_signals);
	signal(SIGINT, exec_signals);
	if (create_pipes(pipeline, shell) == -1)
		return ;
	while (cur)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			err_msg(cmd->name, "failed to create child process", shell, 1);
			return ;
		}
		else if (pids[i] == 0)
			child_process(cur, shell, i);
		cur = cur->next;
		i++;
	}
	parent_process(pipeline, pids, shell);
	
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}

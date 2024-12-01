/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/01 14:29:31 by mglikenf         ###   ########.fr       */
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
}

void	count_cmds(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		cmd->n_cmds++;
		current = current->next;
	}
}

void	display_error_and_return(char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_putendl_fd(strerror(errno), 2);
	return ;
}

void	create_pipe(t_pipe *pipeline, int i)
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
	}
	if (i == 0)
		pipeline->read_end = STDIN_FILENO;
	else
		pipeline->read_end = pipeline->pipefd[i - 1][0];
	if (i == pipeline->n_pipes)
		pipeline->write_end = STDOUT_FILENO;
	else
		pipeline->write_end = pipeline->pipefd[i][1];
}

void	duplicate_fds(t_pipe *pipeline, int i)
{
	if (i > 0)
	{
		if (dup2(pipeline->pipefd[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipeline->read_end);
	}
	if (i < pipeline->n_pipes)
	{
		if (dup2(pipeline->pipefd[i][1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipeline->write_end);
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
	}
	duplicate_fds(pipeline, i);
	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
	{
		if (execve(cmd->name, cmd->args, envp) == -1)
		{
			free_memory(envp);
			display_error_and_return("minishell: execve: ");
		}
	}
}

void	parent_process(t_pipe *pipeline, pid_t pids[MAX_PIPES + 1])
{
	int	i;
	int	status;

	i = 0;
	while (i < pipeline->n_pipes)
	{
		close(pipeline->pipefd[i][0]);
		close(pipeline->pipefd[i][0]);
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
	i = 0;
	while (i <= pipeline->n_pipes)
	{
		create_pipe(pipeline, i);
		pids[i] = fork();
		if (pids[i] == 0)
			child_process(cmd, shell, pipeline, i);
		else if (pids[i] < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		current_cmd = current_cmd->next;
		i++;
	}
	parent_process(pipeline, pids);
}

void	executor(t_command *cmd, t_shell *shell)
{
	t_pipe	*pipeline;

	pipeline = malloc(sizeof(t_pipe));
	if (!pipeline)
	{
		perror("malloc pipeline");
		return ;
	}
	ft_memset(pipeline, 0, sizeof(t_pipe));
	count_cmds(cmd);
	pipeline->n_pipes = cmd->n_cmds - 1;
	// if (pipeline->n_pipes > MAX_PIPES)
	// {
	// 	ft_putendl_fd("pipe limit exceeded", 2);
	// 	free(pipeline);
	// 	return ;
	// }
	if (is_builtin(cmd->name) && cmd->n_cmds < 2)
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

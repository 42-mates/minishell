/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/01 17:30:56 by oprosvir         ###   ########.fr       */
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

void	execute_command(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**envp;

	envp = convert_to_array(shell->env_vars);
	if (!envp)
	{
		shell->exit_status = 1;
		return ;
	}
	// int		pipefd[2];
	// int		wstatus;
	// if (pipe(pipefd) == -1)
	// {
	// 	perror("pipe");
	// 	shell->exit_status = errno;
	// 	return ;
	// }
	pid = fork();
	if (pid < 0)
	{
		// close(pipefd[0]);
		// close(pipefd[1]);
		perror("fork");
		shell->exit_status = errno;
		ft_putendl_fd(strerror(errno), 2);
		return ;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(cmd->args[0], cmd->args, envp) == -1)
		{
			perror("execvp");
			free_memory(envp);
			exit(126);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		shell->exit_status = 1;
	}
	else
	{
		status = 0;
		waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		else
			shell->exit_status = 1;
	}
	free_memory(envp);
}

void	executor(t_command *cmd, t_shell *shell)
{
	if (is_builtin(cmd->name))
		execute_builtin(cmd, shell);
	else
		execute_command(cmd, shell);
}

/*
typedef struct s_pipe
{
	int	n_pipes;
	int	pipefd[MAX_PIPES][2];
	int last_exit_status;
	int	read_end;
	int	write_end;
} 	t_pipe;

void parent_process(t_pipe *pipeline, pid_t pids[], int n_procs)
{
    int i;
    int status;

    // Закрываем оставшиеся файловые дескрипторы
    if (pipeline->n_pipes > 0)
    {
        close(pipeline->pipefd[pipeline->n_pipes - 1][0]);
        close(pipeline->pipefd[pipeline->n_pipes - 1][1]);
    }

    // Ожидаем завершения всех дочерних процессов
    for (i = 0; i < n_procs; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            pipeline->last_exit_status = WEXITSTATUS(status);
    }
}

void child_process(t_command *cmd, t_shell *shell, t_pipe *pipeline, int i)
{
    char    **envp;

    envp = convert_to_array(shell->env_vars);
    if (!envp)
    {
        shell->exit_status = 1;
        exit(shell->exit_status);
    }
    duplicate_fds(pipeline, i);
    // Закрываем все пайпы в дочернем процессе
    for (int j = 0; j < pipeline->n_pipes; j++)
    {
        close(pipeline->pipefd[j][0]);
        close(pipeline->pipefd[j][1]);
    }
    if (is_builtin(cmd->name))
    {
        execute_builtin(cmd, shell);
        exit(shell->exit_status);
    }
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
		{
			child_process(current_cmd, shell, pipeline, i);
			exit(shell->exit_status);
		}
		else if (pids[i] < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		 if (i > 0)
        {
            close(pipeline->pipefd[i - 1][0]);
            close(pipeline->pipefd[i - 1][1]);
        }
		current_cmd = current_cmd->next;
		i++;
	}
	parent_process(pipeline, pids, i);
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
	// if (pipeline->n_pipes > MAX_PIPES)
	// {
	// 	ft_putendl_fd("pipe limit exceeded", 2);
	// 	free(pipeline);
	// 	return ;
	// }
	if (is_builtin(cmd->name) && n_cmds < 2)
		execute_builtin(cmd, shell);
	else
		execute_extern(cmd, shell, pipeline);
	free(pipeline);
}


*/

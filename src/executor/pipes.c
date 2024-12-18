/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 00:46:41 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/18 15:29:27 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	close_pipes(t_pipe *pipeline)
{
	int	i;

	i = 0;
	while (i < pipeline->n_pipes)
	{
		close(pipeline->pipefd[i][0]);
		close(pipeline->pipefd[i][1]);
		i++;
	}
}

int	create_pipes(t_pipe *pipeline, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < pipeline->n_pipes)
	{
		if (pipe(pipeline->pipefd[i]) == -1)
		{
			while (--i >= 0)
			{
				close(pipeline->pipefd[i][0]);
				close(pipeline->pipefd[i][1]);
			}
			perror("minishell: pipe: ");
			shell->exit_status = 1;
			return (-1);
		}
		i++;
	}
	return (0);
}

bool	set_pipeline(t_command *cmd, t_shell *shell)
{
	shell->pipeline.n_pipes = count_cmds(cmd) - 1;
	if (shell->pipeline.n_pipes > MAX_PIPES)
	{
		err_msg("pipe", "pipe limit exceeded", shell, 1);
		shell->exit_status = 1;
		return (false);
	}
	return (true);
}

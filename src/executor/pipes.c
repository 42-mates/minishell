/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 00:46:41 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/07 14:09:29 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			free(pipeline);
			shell->exit_status = 1;
			return (-1);
		}
		i++;
	}
	return (0);
}

t_pipe	*set_pipeline(t_shell *shell, t_command *cmd)
{
	t_pipe	*pipeline;

	pipeline = malloc(sizeof(t_pipe));
	if (!pipeline)
	{
		perror("malloc");
		shell->exit_status = 1;
		return (NULL);
	}
	ft_memset(pipeline, 0, sizeof(t_pipe));
	pipeline->n_pipes = count_cmds(cmd) - 1;
	if (pipeline->n_pipes > MAX_PIPES)
	{
		ft_putendl_fd("too many commands, pipe limit exceeded", 2);
		shell->exit_status = 1;
		free(pipeline);
		return (NULL);
	}
	return (pipeline);
}

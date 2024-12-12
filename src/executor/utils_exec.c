/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:46:05 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/12 08:03:07 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_pipeline(t_pipe *pipeline)
{
	int i;

	i = 0;
	while (i < pipeline->n_pipes)
	{
		close(pipeline->pipefd[i][0]);
		close(pipeline->pipefd[i][1]);
		i++;
	}
	init_pipeline(pipeline);
}

int	count_cmds(t_command *cmd)
{
	t_command	*current;
	int			n_cmds;

	n_cmds = 0;
	current = cmd;
	while (current)
	{
		n_cmds++;
		current = current->next;
	}
	return (n_cmds);
}

/**
 * Sorts an array of environment variables
 * in ascending order based on the variable names
 * using bubble sort
 */
void	sort_env_array(t_env **array)
{
	int		size;
	int		i;
	int		j;
	t_env	*temp;

	size = 0;
	i = 0;
	while (array[size])
		size++;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(array[j]->name, array[j + 1]->name) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	free_array(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

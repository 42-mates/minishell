/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:46:05 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/04 22:47:11 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

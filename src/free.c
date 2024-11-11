/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:31:08 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/10 23:39:02 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env(t_env *lst)
{
	t_env	*temp;

	if (!lst)
		return ;
	while (lst)
	{
		temp = lst;
		lst = lst->next;
		free(temp->name);
		free(temp->value);
		free(temp);
	}
}

void free_shell(t_shell *shell)
{
    if (!shell)
        return;
    free_env(shell->env_vars);
    free(shell);
}

void	free_command(t_command *cmd)
{
	int i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	free(cmd);
}
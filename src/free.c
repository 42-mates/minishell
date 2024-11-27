/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:31:08 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/27 01:32:11 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *lst)
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

int	free_shell(t_shell *shell)
{
	int	exit_status;

	if (!shell)
		return (EXIT_FAILURE);
	exit_status = shell->exit_status;
	free_env(shell->env_vars);
	free(shell);
	return (exit_status);
}

void	free_memory(char **ptr)
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

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

// TODO : нужно изменить под новую структуру
void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_memory(cmd->args);
	free(cmd);
}
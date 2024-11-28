/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:31:08 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/28 22:48:37 by oprosvir         ###   ########.fr       */
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
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->name)
		free(cmd->name);
	if (cmd->args)
		free_memory(cmd->args);
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->append_file)
		free(cmd->append_file);
	if (cmd->delimiter)
		free(cmd->delimiter);
	free(cmd);
}

void	free_commands(t_command *cmd)
{
	t_command	*temp;

	while (cmd)
	{
		temp = cmd;
		cmd = cmd->next;
		free_command(temp);
	}
}

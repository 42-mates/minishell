/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:31:08 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/13 21:07:05 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void free_redirects(t_redirect *redirects)
{
	t_redirect *current;
	t_redirect *next;

	current = redirects;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		free(current);
		current = next;
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->name)
		free(cmd->name);
	if (cmd->args)
		free_array(cmd->args);
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	// if (cmd->delimiter)
		// free(cmd->delimiter);
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

int	free_shell(t_shell *shell)
{
	int	exit_status;

	if (!shell)
		return (EXIT_FAILURE);
	exit_status = shell->exit_status;
	if (shell->pwd)
		free(shell->pwd);
	free_env(shell->env_vars);
	cleanup_pipeline(&shell->pipeline);
	rl_clear_history();
	free(shell);
	return (exit_status);
}

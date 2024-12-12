/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 02:07:27 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/12 07:48:36 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_args(t_token **tokens, t_command *cmd)
{
	if (!cmd->name)
	{
		cmd->name = ft_strdup((*tokens)->value);
		cmd->args = append_to_array(NULL, cmd->name);
	}
	else
		cmd->args = append_to_array(cmd->args, (*tokens)->value);
	(*tokens) = (*tokens)->next;
}

bool	parse_heredoc(t_token **tokens, t_command *cmd, t_shell *shell)
{
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
	{
		err_msg(NULL, "syntax error near unexpected token", shell, 2);
		return (false);
	}
	cmd->delimiter = ft_strdup((*tokens)->next->value);
	if (!cmd->delimiter)
		return (false);
	(*tokens) = (*tokens)->next->next;
	return (true);
}

bool	parse_redirects(t_token **tokens, t_command *cmd, t_shell *shell)
{
	t_redirect *new_redir;
	t_redirect *last;
	
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
	{
		err_msg(NULL, "syntax error near unexpected token", shell, 2);
		return (false);
	}
	new_redir = malloc(sizeof(t_redirect));
	if (!new_redir)
		return (false);
	new_redir->filename = ft_strdup((*tokens)->next->value);
	if (!new_redir->filename)
		return (free(new_redir), false);
	if ((*tokens)->type == R_OUTPUT)
		new_redir->type = R_OUTPUT;
	else if ((*tokens)->type == R_APPEND)
		new_redir->type = R_APPEND;
	else if ((*tokens)->type == R_INPUT)
		new_redir->type = R_INPUT;
	new_redir->next = NULL;
	if (!cmd->redirects)
		cmd->redirects = new_redir;
	else
	{
		last = cmd->redirects;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
	(*tokens) = (*tokens)->next->next;
	return (true);
}

bool	parse_pipe(t_token **tokens, t_command **cmd, t_shell *shell)
{
	if (!(*cmd) || !(*tokens)->next || (*tokens)->next->type != WORD)
	{
		err_msg(NULL, "syntax error near unexpected token `|`", shell, 2);
		return (false);
	}
	(*cmd)->next = init_command(shell);
	if (!(*cmd)->next)
		return (false);
	*cmd = (*cmd)->next;
	*tokens = (*tokens)->next;
	return (true);
}

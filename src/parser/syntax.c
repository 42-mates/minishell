/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 02:07:27 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/17 15:41:07 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_args(t_token **tokens, t_command *cmd)
{
	if ((*tokens)->value && (*tokens)->value[0] == '\0')
	{
		(*tokens) = (*tokens)->next;
		return ;
	}
	if (!cmd->name)
	{
		cmd->name = ft_strdup((*tokens)->value);
		cmd->args = append_to_array(NULL, cmd->name);
	}
	else
		cmd->args = append_to_array(cmd->args, (*tokens)->value);
	(*tokens) = (*tokens)->next;
}

static t_redirect	*init_redirect(t_token *token)
{
	t_redirect	*new_redir;

	new_redir = malloc(sizeof(t_redirect));
	if (!new_redir)
		return (NULL);
	new_redir->filename = ft_strdup(token->next->value);
	if (!new_redir->filename)
	{
		free(new_redir);
		return (NULL);
	}
	if (token->type == R_OUTPUT)
		new_redir->type = R_OUTPUT;
	else if (token->type == R_APPEND)
		new_redir->type = R_APPEND;
	else if (token->type == R_INPUT)
		new_redir->type = R_INPUT;
	else if (token->type == R_HEREDOC)
		new_redir->type = R_HEREDOC;
	new_redir->next = NULL;
	return (new_redir);
}

bool	parse_redirects(t_token **tokens, t_command *cmd, t_shell *shell)
{
	t_redirect	*new_redir;
	t_redirect	*last;

	if (!(*tokens)->next || (*tokens)->next->type != WORD)
	{
		err_msg(NULL, "syntax error near unexpected token", shell, 2);
		return (false);
	}
	new_redir = init_redirect(*tokens);
	if (!new_redir)
		return (false);
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
	if (!(*cmd) || !(*tokens)->next)
	{
		err_msg(NULL, "syntax error near unexpected token `|`", shell, 2);
		return (false);
	}
	(*cmd)->next = init_command(shell);
	if (!(*cmd)->next)
		return (false);
	*cmd = (*cmd)->next;
	*tokens = (*tokens)->next;
	if (is_redirect((*tokens)->type))
	{
		if (!parse_redirects(tokens, *cmd, shell))
			return (false);
	}
	else if ((*tokens)->type == WORD)
		parse_args(tokens, *cmd);
	return (true);
}

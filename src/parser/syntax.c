/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 02:07:27 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/11 20:32:28 by oprosvir         ###   ########.fr       */
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

bool	parse_redirects(t_token **tokens, t_command *cmd, t_shell *shell)
{
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
	{
		err_msg(NULL, "syntax error near unexpected token", shell, 2);
		return (false);
	}
	if ((*tokens)->type == REDIRECT_OUT)
		cmd->output_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == APPEND)
		cmd->append_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == REDIRECT_IN)
		cmd->input_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == HEREDOC)
		cmd->delimiter = ft_strdup((*tokens)->next->value);
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

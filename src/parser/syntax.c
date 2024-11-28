/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 02:07:27 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/28 22:49:05 by oprosvir         ###   ########.fr       */
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

int	parse_redirects(t_token **tokens, t_command *cmd, t_shell *shell)
{
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
		return (err_msg(NULL, "syntax error near unexpected token", shell, 2), -1);
	if ((*tokens)->type == REDIRECT_OUT)
		cmd->output_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == APPEND)
		cmd->append_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == REDIRECT_IN)
		cmd->input_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == HEREDOC)
		cmd->delimiter = ft_strdup((*tokens)->next->value);
	(*tokens) = (*tokens)->next->next;
	return (0);
}

int	parse_pipe(t_token **tokens, t_command **current, t_shell *shell)
{
	if (!(*current) || !(*tokens)->next || (*tokens)->next->type != WORD)
		return (err_msg(NULL, "syntax error near unexpected token `|`", shell, 2), -1);
	(*current)->next = init_command(shell);
	if (!(*current)->next)
		return (-1);
	*current = (*current)->next;
	*tokens = (*tokens)->next;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:22:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/07 15:27:02 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(t_shell *shell)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (set_status(shell, 1));
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->output_file = NULL;
	cmd->input_file = NULL;
	cmd->append_file = NULL;
	cmd->delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

bool	is_redirect(t_token_type type)
{
	return (type == REDIRECT_OUT || type == REDIRECT_IN || type == APPEND
		|| type == HEREDOC);
}

bool	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 18:22:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/12 07:44:49 by oprosvir         ###   ########.fr       */
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
	cmd->redirects = NULL;
	cmd->delimiter = NULL;
	cmd->tmp_file_path = NULL;
	cmd->next = NULL;
	return (cmd);
}

bool	is_redirect(t_token_type type)
{
	return (type == R_OUTPUT || type == R_INPUT || type == R_APPEND);
}

bool	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

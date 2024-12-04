/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:12:56 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/04 22:18:58 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * return codes (shell->exit_status):
 * 2 : invalid option passed (e.g., "pwd -z")
 * 1 : error retrieving the current working directory (e.g., `getcwd` fails)
 * 0 : success (current working directory printed)
 */
int ft_pwd(t_command *cmd)
{
	char *cwd;

	if (cmd->args[1] && cmd->args[1][0] == '-' && cmd->args[1][1] != '\0')
		return (cmd_err("pwd", cmd->args[1], "invalid option", 2));
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (cmd_err("pwd", NULL, strerror(errno), ERROR));
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (SUCCESS);
}

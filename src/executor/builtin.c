/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/07 12:49:58 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(const char *cmd_name)
{
    return (ft_strcmp(cmd_name, "cd") == 0 ||
            ft_strcmp(cmd_name, "echo") == 0 ||
            ft_strcmp(cmd_name, "pwd") == 0 ||
            ft_strcmp(cmd_name, "export") == 0 ||
            ft_strcmp(cmd_name, "unset") == 0 ||
            ft_strcmp(cmd_name, "env") == 0 ||
            ft_strcmp(cmd_name, "exit") == 0);
}

void	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->name, "exit") == 0)
		ft_exit(cmd, shell);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(cmd->name, "echo") == 0)
		ft_echo(cmd, shell);
	else if (ft_strcmp(cmd->name, "env") == 0)
		ft_env(cmd, shell);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		ft_unset(cmd, shell);
	else if (ft_strcmp(cmd->name, "export") == 0)
		ft_export(cmd, shell);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		ft_cd(cmd, shell);
}

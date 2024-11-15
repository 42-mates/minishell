/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:13:08 by mglikenf          #+#    #+#             */
/*   Updated: 2024/11/15 16:22:20 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void    ft_exit(t_command *cmd, t_shell *shell)
{
    if (cmd->args[1] && cmd->args[2] == NULL)
        shell->exit_status = ft_atoi(cmd->args[1]);
    ft_putendl_fd(cmd->name, 1);
    exit(shell->exit_status);
} */


void	ft_exit(t_command *cmd, t_shell *shell)
{
	free_command(cmd);
	exit(free_shell(shell));
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:28:03 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/13 22:39:25 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_command *cmd, t_shell *shell)
{
	free_command(cmd);
	exit(free_shell(shell));
}

// note : нужна реализация
void	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->name, "exit") == 0)
		ft_exit(cmd, shell);
}
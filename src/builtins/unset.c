/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:41:18 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/04 19:03:52 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* return codes (shell->exit_status):
* 2 : one of arguments is option
* 0 : success / no args
*/
int	ft_unset(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
		return (SUCCESS);
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-')
			return (cmd_err("unset", cmd->args[i], "invalid option", 2));
		i++;
	}
	i = 1;
	while (cmd->args[i])
	{
		remove_var(&(shell->env_vars), cmd->args[i]);
		i++;
	}
	return (SUCCESS);
}

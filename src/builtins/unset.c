/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:41:18 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/29 17:32:48 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* return codes (shell->exit_status):
* 2 : one of arguments is option
* 0 : success / no args
*/

// TODO : check with pipes
void	ft_unset(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-')
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd(": invalid option", 2);
			shell->exit_status = 2;
			return ;
		}
		i++;
	}
	i = 1;
	while (cmd->args[i])
	{
		remove_var(&(shell->env_vars), cmd->args[i]);
		i++;
	}
	shell->exit_status = 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:41:18 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/21 11:56:43 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Unsets (removes) environment variables.
 *
 * This function removes variables from the shell's environment if they
 * are specified as arguments. If an argument starts with a dash ('-'),
 * it is treated as an invalid option.
 *
 * @return Return codes for shell->exit_status:
 * - 2: One of the arguments is an invalid option.
 * - 0: Success (no arguments or successful execution).
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

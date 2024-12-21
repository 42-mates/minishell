/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:54:56 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/21 12:01:21 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 *  @return Return codes for shell->exit_status:
 * - 2: More than one argument passed.
 * - 1: Environment variables are empty.
 * - 0: Success (all environment variables printed).
 */
int	ft_env(t_command *cmd, t_shell *shell)
{
	t_env	*current;

	if (cmd->args[1])
		return (cmd_err("env", NULL, "no options or arguments are allowed", 2));
	if (!shell->env_vars)
		return (cmd_err("env", NULL, "environment is empty", ERROR));
	current = shell->env_vars;
	while (current)
	{
		ft_putstr_fd(current->name, 1);
		ft_putstr_fd("=", 1);
		if (current->value)
			ft_putstr_fd(current->value, 1);
		ft_putstr_fd("\n", 1);
		current = current->next;
	}
	return (SUCCESS);
}

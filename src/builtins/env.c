/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:54:56 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/04 20:24:29 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* return codes (shell->exit_status):
* 2 : more than one argument passed
* 1 : environment variables are empty
* 0 : success
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
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
	return (SUCCESS);
}

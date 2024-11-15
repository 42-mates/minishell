/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:54:56 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/15 15:55:18 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
return codes (shell->exit_status):
2 : more than one argument passed
1 : environment variables are empty
0 : success
*/

void	ft_env(t_command *cmd, t_shell *shell)
{
	t_env	*current;

	if (cmd->args[1])
	{
		shell->exit_status = 2;
		ft_putstr_fd("env: no options or arguments are allowed\n", 2);
		return ;
	}
	if (!shell->env_vars)
	{
		shell->exit_status = 1;
		ft_putstr_fd("env: environment is empty\n", 2);
		return ;
	}
	current = shell->env_vars;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
	shell->exit_status = 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:31:52 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/28 18:59:06 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shlvl(t_env **env_vars)
{
	char	*shlvl_str;
	char    *new_shlvl;
	int		shlvl;

    shlvl = 0;
	shlvl_str = getenv_lst("SHLVL", *env_vars);
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	if (shlvl < 0)
		shlvl = 0;
	else
		shlvl++;
	new_shlvl = ft_itoa(shlvl);
	setenv_lst("SHLVL", new_shlvl, env_vars);
	free(new_shlvl);
}

t_shell	*init_shell(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Error: This program does not accept arguments.\n", 2);
		return (NULL);
	}
	setup_signals();
	welcome_message();
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_vars = init_env(envp);
	if (!shell->env_vars)
	{
		free(shell);
		return (NULL);
	}
	shell->exit_status = 0;
	init_shlvl(&shell->env_vars);
	return (shell);
}

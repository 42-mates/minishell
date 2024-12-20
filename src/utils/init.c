/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:31:52 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/19 16:28:23 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_args(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
		ft_putendl_fd("minishell: warning: arguments ignored", 2);
	if (!envp || !*envp)
	{
		ft_putstr_fd("minishell: warning: ", 2);
		ft_putendl_fd("no environment variables set", 2);
	}
}

static char	*init_pwd(t_env **env_vars)
{
	char	*pwd;
	char	*cwd;

	pwd = getenv_lst("PWD", *env_vars);
	if (!pwd)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			return (ft_strdup("."));
		setenv_lst("PWD", cwd, env_vars);
		pwd = ft_strdup(cwd);
		free(cwd);
	}
	else
		pwd = ft_strdup(pwd);
	return (pwd);
}

static void	init_shlvl(t_env **env_vars)
{
	char	*shlvl_str;
	char	*new_shlvl;
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

void	init_pipeline(t_pipe *pipeline)
{
	int	i;

	i = 0;
	pipeline->n_pipes = 0;
	while (i < MAX_PIPES)
	{
		pipeline->pipefd[i][0] = -1;
		pipeline->pipefd[i][1] = -1;
		i++;
	}
}

t_shell	*init_shell(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	check_args(argc, argv, envp);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->exit_status = 0;
	shell->env_vars = init_env(envp);
	shell->pwd = init_pwd(&shell->env_vars);
	init_pipeline(&shell->pipeline);
	init_shlvl(&shell->env_vars);
	return (shell);
}

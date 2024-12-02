/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 01:12:37 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/02 10:58:37 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_oldpwd(t_shell *shell)
{
	char *oldpwd;

	oldpwd = getenv_lst("OLDPWD", shell->env_vars);
	if (!oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return NULL;
	}
	return ft_strdup(oldpwd);
}

static void update_pwd(t_shell *shell)
{
	char *old_pwd;
	char *new_pwd;

	old_pwd = getenv_lst("PWD", shell->env_vars);
	if (old_pwd)
		setenv_lst("OLDPWD", old_pwd, &(shell->env_vars));
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		setenv_lst("PWD", new_pwd, &(shell->env_vars));
		free(new_pwd);
	}
}

void ft_cd(t_command *cmd, t_shell *shell)
{
	char *path;

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return;
	}

	if (!cmd->args[1])
	{
		path = getenv_lst("HOME", shell->env_vars);
		if (!path || ft_strlen(path) == 0)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			shell->exit_status = 1;
			return ;
		}
		path = ft_strdup(path);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		path = expand_oldpwd(shell);
		ft_putendl_fd(path, 1); // show new path when 'cd -'
	}
	else
		path = ft_strdup(cmd->args[1]);

	if (chdir(path) != 0)
	{
		errmsg_cmd("cd", path, strerror(errno));
		// exit status (errno ?)
		free(path);
		return ;
	}
	update_pwd(shell);
	free(path);
	shell->exit_status = 0;
}

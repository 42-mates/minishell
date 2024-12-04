/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 01:12:37 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/04 13:19:04 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *expand_oldpwd(t_shell *shell)
{
	char *oldpwd;

	oldpwd = getenv_lst("OLDPWD", shell->env_vars);
	if (!oldpwd)
		return (NULL);
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
	else
        setenv_lst("PWD", "?", &(shell->env_vars)); // not sure about it TODO : test w/export
}

static void *cd_err(char *msg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putendl_fd(msg, 2);
	return (NULL);
}

static char *cd_path(t_command *cmd, t_shell *shell)
{
	char *path;

	path = NULL;
	if (!cmd->args[1])
	{
		path = getenv_lst("HOME", shell->env_vars);
		if (!path || ft_strlen(path) == 0)
			return (cd_err("HOME not set"));
		return ft_strdup(path);
	}
	if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		path = expand_oldpwd(shell);
		if (!path)
			return (cd_err("OLDPWD not set"));
		ft_putendl_fd(path, 1);
		return (path);
	}
	return ft_strdup(cmd->args[1]);
}

int ft_cd(t_command *cmd, t_shell *shell)
{
	char *path;

	if (cmd->args[1] && cmd->args[2])
		return (cmd_err("cd", NULL, "too many arguments", ERROR));
	path = cd_path(cmd, shell);
	if (!path)
		return (ERROR);
	if (chdir(path) != 0)
	{
		cmd_err("cd", path, strerror(errno), ERROR);
		free(path);
		return (ERROR);
	}
	update_pwd(shell);
	free(path);
	return (SUCCESS);
}

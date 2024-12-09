/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:18:08 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/09 16:57:23 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	**list_to_env_array(t_env *env_list)
{
	int		size;
	t_env	*current;
	t_env	**array;
	int		i;

	size = 0;
	current = env_list;
	i = 0;
	while (current)
	{
		size++;
		current = current->next;
	}
	array = malloc(sizeof(t_env *) * (size + 1));
	if (!array)
		return (NULL);
	current = env_list;
	while (current)
	{
		array[i++] = current;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

static int	print_env_vars(t_env *env_list)
{
	t_env	**array;
	int		i;

	array = list_to_env_array(env_list);
	if (!array)
		return (cmd_err("export", NULL, "failed to fetch env vars", ERROR));
	sort_env_array(array);
	i = 0;
	while (array[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(array[i]->name, 1);
		if (array[i]->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(array[i]->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		i++;
	}
	free(array);
	return (SUCCESS);
}

static bool	is_valid(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (false);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static int	export_value(char *arg, char *equals_sign, t_shell *shell)
{
	char	*name;
	char	*value;
	int		status;

	status = 0;
	name = ft_substr(arg, 0, equals_sign - arg);
	value = ft_strdup(equals_sign + 1);
	if (!is_valid(name))
		status = cmd_err("export", arg, "not a valid identifier", 1);
	else
		setenv_lst(name, value, &(shell->env_vars));
	free(name);
	free(value);
	return (status);
}

int	ft_export(t_command *cmd, t_shell *shell)
{
	int		i;
	int		status;
	char	*equals_sign;

	i = 1;
	status = 0;
	if (!cmd->args[1])
		return (print_env_vars(shell->env_vars));
	while (cmd->args[i])
	{
		equals_sign = ft_strchr(cmd->args[i], '=');
		if (equals_sign)
			status |= export_value(cmd->args[i], equals_sign, shell);
		else
		{
			if (!is_valid(cmd->args[i]))
				status = cmd_err("export", cmd->args[i],
						"not a valid identifier", 1);
			else
				setenv_lst(cmd->args[i], "", &(shell->env_vars));
		}
		i++;
	}
	return (status);
}

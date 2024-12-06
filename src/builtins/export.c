/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:18:08 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/06 23:35:41 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Sorts an array of environment variables
 * in ascending order based on the variable names
 * using bubble sort
 */
static void	sort_env_array(t_env **array)
{
	int		size;
	int		i;
	int		j;
	t_env	*temp;

	size = 0;
	i = 0;
	while (array[size])
		size++;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(array[j]->name, array[j + 1]->name) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

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
bool is_valid_identifier(const char *str)
{
	int i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (false);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return false;
		i++;
	}
	return (true);
}

// TODO : need to test and bugfix
// export var shouldn't add var to env but to export
// export HELLO="123 A-" makes 2 tokens
int	ft_export(t_command *cmd, t_shell *shell)
{
	int		i;
	int		status;
	char	*equals_sign;
	char	*name;
	char	*value;

	i = 1;
	status = 0;
	if (!cmd->args[1])
		return (print_env_vars(shell->env_vars));
	while (cmd->args[i])
	{
		equals_sign = ft_strchr(cmd->args[i], '=');
		if (equals_sign)
		{
			name = ft_substr(cmd->args[i], 0, equals_sign - cmd->args[i]);
			value = ft_strdup(equals_sign + 1);
			if (!is_valid_identifier(name))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				status = 1;
			}
			else
				setenv_lst(name, value, &(shell->env_vars));
			free(name);
			free(value);
		}
		else
		{
			if (!is_valid_identifier(cmd->args[i]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				status = 1;
			}
			else
				setenv_lst(cmd->args[i], "", &(shell->env_vars));
		}
		i++;
	}
	return (status);
}
 /*
int	ft_export(t_command *cmd, t_shell *shell)
{
	int		i;
	int		status;
	char	*equals_sign;
	char	*name;
	char	*value;

	i = 1;
	status = 0;
	if (!cmd->args[1])
		return (print_env_vars);
	while (cmd->args[i])
	{
		equals_sign = ft_strchr(cmd->args[i], '=');
		if (equals_sign)
		{
			name = ft_substr(cmd->args[i], 0, equals_sign - cmd->args[i]);
			value = ft_strdup(equals_sign + 1);
			if (!is_valid_identifier(name))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				status = 1;
			}
			else
				setenv_lst(name, value, &(shell->env_vars));
			free(name);
			free(value);
		}
		else
		{
			if (!is_valid_identifier(cmd->args[i]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				status = 1;
			}
			else
				setenv_lst(cmd->args[i], "", &(shell->env_vars));
		}
		i++;
	}
	shell->exit_status = status;
	return ;
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:25:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/16 15:58:26 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_env_var *get_env_var(t_env_var *env_list, const char *name);
// void set_env_var(t_env_var **env_list, const char *name, const char *value);
// void unset_env_var(t_env_var **env_list, const char *name);

char	*getenv_lst(const char *name, t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static int	append_node(t_env **list, const char *env_entry)
{
	t_env	*new_node;
	t_env	*current;
	char	*equals_sign;

	equals_sign = ft_strchr(env_entry, '=');
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (1);
	new_node->name = ft_substr(env_entry, 0, equals_sign - env_entry);
	new_node->value = ft_strdup(equals_sign + 1);
	new_node->next = NULL;
	if (*list == NULL)
		*list = new_node;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (0);
}

// initializes t_env list with envp environment variables
// passed at program startup

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*env_list;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		if (append_node(&env_list, envp[i]) != 0)
		{
			free_env(env_list);
			return (NULL);
		}
		i++;
	}
	return (env_list);
}

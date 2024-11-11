/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:09:23 by codespace         #+#    #+#             */
/*   Updated: 2024/11/11 18:46:28 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_env_var *get_env_var(t_env_var *env_list, const char *name);
// void set_env_var(t_env_var **env_list, const char *name, const char *value);
// void unset_env_var(t_env_var **env_list, const char *name);

static int	env_list_size(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

static void	*free_envp_array(char **envp, int n)
{
	while (n > 0)
		free(envp[--n]);
	free(envp);
	return (NULL);
}

char	*create_env_string(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	char *str = malloc(name_len + 1 + value_len + 1); // name + '=' + value	+ '\0'
	if (!str)
		return (NULL);
	ft_memcpy(str, name, name_len);
	str[name_len] = '=';
	ft_memcpy(str + name_len + 1, value, value_len);
	str[name_len + value_len + 1] = '\0';
	return (str);
}

char	**convert_env_list_to_array(t_env *env_list)
{
	int		i;
	int		count;
	char	**envp;
	t_env	*current;

	i = 0;
	count = env_list_size(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = env_list;
	while (current)
	{
		envp[i] = create_env_string(current->name, current->value);
		if (!envp[i])
			return (free_envp_array(envp, i));
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void	add_node(t_env **list, const char *name, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	// if (!new_node)
	//    return (1);
	// add : обработка ошибок
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = *list;
	*list = new_node;
}

// initializes t_env list with envp environment variables
// passed at program startup

t_env	*init_env(char **envp)
{
	int i;
	t_env *env_list;
	char *equals_sign;
	char *name;
	char *value;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		equals_sign = ft_strchr(envp[i], '=');
		if (equals_sign)
		{
			name = ft_substr(envp[i], 0, equals_sign - envp[i]);
			value = ft_strdup(equals_sign + 1);
			add_node(&env_list, name, value);
			// add : обработка ошибок
			free(name);
			free(value);
		}
		i++;
	}
	return (env_list);
}
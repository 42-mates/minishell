/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:09:23 by codespace         #+#    #+#             */
/*   Updated: 2024/11/10 01:32:00 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_env_var *get_env_var(t_env_var *env_list, const char *name);
// void set_env_var(t_env_var **env_list, const char *name, const char *value);
// void unset_env_var(t_env_var **env_list, const char *name);

void add_node(t_env **list, const char *name, const char *value)
{
	t_env *new_node;
	
    new_node = malloc(sizeof(t_env));
	//if (!new_node)
    //    return 1;
	// add : обработка ошибок
    new_node->name = ft_strdup(name);
    new_node->value = ft_strdup(value);
    new_node->next = *list;
    *list = new_node;
}

// initializes t_env list with envp environment variables
// passed at program startup

t_env *init_env(char **envp)
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
    return env_list;
}
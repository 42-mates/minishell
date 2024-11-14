/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:25:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/14 18:37:30 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_env_var *get_env_var(t_env_var *env_list, const char *name);
// void set_env_var(t_env_var **env_list, const char *name, const char *value);
// void unset_env_var(t_env_var **env_list, const char *name);

static int append_node(t_env **list, const char *name, const char *value)
{
    t_env	*new_node;
    t_env	*current;

    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return (1);
    new_node->name = ft_strdup(name);
    new_node->value = ft_strdup(value);
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
            if (append_node(&env_list, name, value) != 0)
            {
                free(name);
                free(value);
                free_env(env_list);
                return (NULL);
            }
            free(name);
            free(value);
        }
        i++;
    }
    return (env_list);
}

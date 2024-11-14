/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:22:28 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/14 20:10:29 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	list_size(t_env *env_list)
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

static void	*free_array(char **envp, int n)
{
	while (n > 0)
		free(envp[--n]);
	free(envp);
	return (NULL);
}

static char	*create_string(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*str;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	str = malloc(name_len + 1 + value_len + 1); // name + '=' + value	+ '\0'
	if (!str)
		return (NULL);
	ft_memcpy(str, name, name_len);
	str[name_len] = '=';
	ft_memcpy(str + name_len + 1, value, value_len);
	str[name_len + value_len + 1] = '\0';
	return (str);
}

char	**convert_to_array(t_env *env_list)
{
	int		i;
	int		count;
	char	**envp;
	t_env	*current;

	i = 0;
	count = list_size(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = env_list;
	while (current)
	{
		envp[i] = create_string(current->name, current->value);
		if (!envp[i])
			return (free_array(envp, i));
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char	*ft_getenv(const char *name, char **envp)
{
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, ft_strlen(name)) == 0)
		{
			value = envp[i] + ft_strlen(name) + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

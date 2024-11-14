/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:35:00 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/13 23:52:22 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*find_relative_path(char *cmd, char **envp)
{
	char	*path;
	char	**split_paths;
	char	*path_slash;
	int		i;

	split_paths = ft_split(ft_getenv("PATH", envp), ':');
	i = 0;
	while (split_paths[i])
	{
		path_slash = ft_strjoin(split_paths[i], "/");
		path = ft_strjoin(path_slash, cmd);
		free(path_slash);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	free_memory(split_paths);
	return (NULL);
}

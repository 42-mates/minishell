/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:35:00 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/28 17:56:14 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_paths(char **paths, char *cmd_name)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd_name);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// TODO : malloc allocation errors
char	*get_path(char *cmd_name, t_env *env_list)
{
	char	*path;
	char	**split_paths;
	char	*full_path;

	if (access(cmd_name, X_OK) == 0)
		return (ft_strdup(cmd_name));
	path = getenv_lst("PATH", env_list);
	if (!path)
		return (NULL);
	split_paths = ft_split(path, ':');
	if (!split_paths)
		return (NULL);
	full_path = search_in_paths(split_paths, cmd_name);
	free_memory(split_paths);
	return (full_path);
}

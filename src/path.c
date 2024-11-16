/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 21:35:00 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/16 13:19:35 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	check_absolute_path(t_command *cmd, char **envp)
{
	if (cmd->args[0][0] == '/')
	{
		if (access(cmd->args[0], X_OK) == 0)
			cmd->path = cmd->args[0];
		else
		{
			// display_error(cmd->args[0]);
			perror(cmd->args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
		find_relative_path(cmd, envp);
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
*/

char	*get_full_exec_path(t_command *cmd, char **envp)
{
	char	*path;
	char	**split_paths;
	char	*full_path;
	int		i;

	if (access(cmd->args[0], X_OK) == 0)
		return (ft_strdup(cmd->args[0])); // Если абсолютный путь корректен
	path = ft_getenv("PATH", envp);
	if (!path)
		return (NULL);
	split_paths = ft_split(path, ':');
	i = 0;
	while (split_paths[i])
	{
		full_path = ft_strjoin(split_paths[i], "/");
		full_path = ft_strjoin(full_path, cmd->args[0]);
		// printf("%s\n", full_path);
		if (access(full_path, X_OK) == 0)
		{
			free_memory(split_paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_memory(split_paths);
	return (NULL);
}

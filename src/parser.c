/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/16 12:57:27 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parser(char *line)
{
	char		**tokens;
	t_command	*cmd;

	tokens = ft_strtok(line, TOKEN_DELIM);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_printf("allocation error\n");
		exit(EXIT_FAILURE);
	}
	cmd->name = tokens[0];
	exec_path = get_full_exec_path(cmd, envp);
	if (!exec_path)
	{
		ft_putstr_fd(cmd->name, 2);
		ft_putstr_fd(": command not found\n", 2);
		shell->exit_status = 127;
		free_memory(envp);
		return ;
	}
	cmd->args = tokens;
	cmd->path = NULL;
	return (cmd);
}

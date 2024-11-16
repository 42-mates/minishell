/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/16 13:19:09 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parser(char *line, t_shell *shell)
{
	char		**tokens;
	t_command	*cmd;
	char		*exec_path;
	char		**envp;

	tokens = ft_strtok(line, TOKEN_DELIM);
	if (!tokens || !tokens[0])
	{
		free(tokens);
		return (NULL);
	}
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_printf("allocation error\n");
		exit(EXIT_FAILURE);
	}
	envp = convert_to_array(shell->env_vars);
	cmd->name = tokens[0];
	cmd->args = tokens;
	if (!is_builtin(cmd->name))
	{
		exec_path = get_full_exec_path(cmd, envp);
		if (!exec_path)
		{
			ft_putstr_fd(tokens[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			shell->exit_status = 127;
			free_memory(tokens);
			free(cmd);
			return (NULL);
		}
		cmd->name = exec_path;
		tokens[0] = exec_path;
		cmd->args = tokens;
	}
	
	return (cmd);
}

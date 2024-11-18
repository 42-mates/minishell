/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/18 15:05:44 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void handle_redirections(char *command)
{
    // Parse and apply <, >, <<, and >> redirections
}

char *expand_env_vars(char *input)
{
    // Parse and replace environment variables (e.g., $USER)
}

void handle_pipes(char **commands)
{
    // Create a pipeline if commands contain `|`
}
*/

t_command *init_command(void)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        ft_printf("allocation error\n");
        return (NULL);
    }
    cmd->name = NULL;
    cmd->args = NULL;
    cmd->output_file = NULL;
    cmd->input_file = NULL;
    cmd->next = NULL;
    return cmd;
}

t_command *parser(char *line, t_shell *shell)
{
    char **commands;
    t_command *head = NULL;
    t_command *current = NULL;
    int i = 0;

    commands = ft_split(line, '|'); // Разделяем команды по '|'
    while (commands[i])
    {
        t_command *new_cmd = init_command();
        new_cmd->args = ft_strtok(commands[i], TOKEN_DELIM);
        new_cmd->name = new_cmd->args[0];

        if (!is_builtin(new_cmd->name))
        {
            char *exec_path = get_path(new_cmd, shell->env_vars);
            if (!exec_path)
            {
                ft_putstr_fd(new_cmd->name, 2);
                ft_putstr_fd(": command not found\n", 2);
                shell->exit_status = 127;
                free(new_cmd);
                continue;
            }
            new_cmd->name = exec_path;
            new_cmd->args[0] = exec_path;
        }

        if (!head)
            head = new_cmd;
        else
            current->next = new_cmd;

        current = new_cmd;
        i++;
    }
    free_memory(commands);
    return head;
}

/*
t_command	*parser(char *line, t_shell *shell)
{
	char		**tokens;
	t_command	*cmd;
	char		*exec_path;

	tokens = ft_strtok(line, TOKEN_DELIM);
	if (!tokens || !tokens[0])
	{
		free(tokens);
		return (NULL);
	}
	cmd = init_command();
	cmd->name = tokens[0];
	cmd->args = tokens;
	if (!is_builtin(cmd->name))
	{
		exec_path = get_path(cmd, shell->env_vars);
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
}*/

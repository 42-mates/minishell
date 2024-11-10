/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:44:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/10 22:34:33 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// note : заглушка для начала работы executor + pwd, echo
static int is_builtin(const char *cmd_name)
{
    const char *builtins[] = {"cd", "export", "unset", "env", "exit", NULL};
    for (int i = 0; builtins[i]; i++)
	{
        if (ft_strcmp(cmd_name, builtins[i]) == 0)
            return 1;
    }
    return 0;
}

// note : нужна реализация
void execute_builtin(t_command *cmd, t_env *env_list)
{
	(void)cmd;
	(void)env_list;
}

void	execute_command(t_command *cmd, t_env *env_list)
{
	pid_t	pid;
	int		status;

	(void)env_list;
	pid = fork();
	if (pid == 0)
	{
		if (execvp(cmd->name, cmd->args) == -1)
			perror("execvp");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		}
		while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}
 
void	executor(t_command *cmd, t_env *env_list)
{
	if (is_builtin(cmd->name))
		execute_builtin(cmd, env_list);
    else
		execute_command(cmd, env_list);
}

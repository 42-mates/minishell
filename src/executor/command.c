/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:25:25 by alex              #+#    #+#             */
/*   Updated: 2024/08/01 15:51:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	**get_command_args(cmd_node *node, int *argc)
{
	int			i;
	cmd_node	*arg_node;
	char		**argv;

	arg_node = node;
	i = 0;
	while (arg_node != NULL && (arg_node->type == NODE_ARGUMENT || arg_node->type == NODE_CMDPATH))
	{
		arg_node = arg_node->right;
		i++;
	}
	argv = (char**)malloc(sizeof(char*) * (i + 1));
	if (!(argv))
		return (NULL);
	arg_node = node;
	i = 0;
	while (arg_node != NULL && (arg_node->type == NODE_ARGUMENT || arg_node->type == NODE_CMDPATH))
	{
		argv[i] = (char*)malloc(strlen(arg_node->data) + 1);
		strcpy(argv[i], arg_node->data);
		arg_node = arg_node->right;
		i++;
	}
	argv[i] = NULL;
	*argc = i;
	return (argv);
}

int init_command_internal(cmd_node* node, cmd_internal* cmd, bool stdin_pipe, bool stdout_pipe, int pipe_read, int pipe_write, char *redirect_in, char *redirect_out)
{
	if (node == NULL || node->type != NODE_CMDPATH)
	{
		cmd->argc = 0;
		return (-1);
	}
	cmd->argv = get_command_args(node, &cmd->argc);
	cmd->stdin_pipe = stdin_pipe;
	cmd->stdout_pipe = stdout_pipe;
	cmd->pipe_read = pipe_read;
	cmd->pipe_write = pipe_write;
	cmd->redirect_in = redirect_in;
    cmd->redirect_out = redirect_out;
	return 0;
}

void destroy_command_internal(cmd_internal* cmd)
{
	int i;
	for (i = 0; i < cmd->argc; i++)
		free(cmd->argv[i]);
	free(cmd->argv);
	cmd->argc = 0;
}

void execute_command_internal(cmd_internal* cmd)
{
	int     stdoutfd;
	pid_t   pid;

	if (cmd->argc < 0)
		return;
	if((pid = fork()) == 0) {
		stdoutfd = dup(STDOUT_FILENO);
		if (cmd->stdin_pipe)
			dup2(cmd->pipe_read, STDIN_FILENO);
		if (cmd->stdout_pipe)
			dup2(cmd->pipe_write, STDOUT_FILENO);
		if (cmd->redirect_out) {
			//printf("rediret to %s\n", cmd->redirect_out);
            int fd = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_TRUNC,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd == -1) {
                perror(cmd->redirect_out);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
        }
		if (execvp(cmd->argv[0], cmd->argv) == -1)
		{
			dup2(stdoutfd, STDOUT_FILENO);
			printf("Command not found: \'%s\'\n", cmd->argv[0]);
			exit(1);
		}
	}
	else if (pid < 0) {
		perror("fork");
		return;
	}
	while (waitpid(pid, NULL, 0) <= 0);
	return;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:53:48 by alex              #+#    #+#             */
/*   Updated: 2024/07/30 14:48:33 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void execute_simple_command(cmd_node* node, bool stdin_pipe, bool stdout_pipe, int pipe_read, int pipe_write)
{
    cmd_internal cmd;
    init_command_internal(node, &cmd, stdin_pipe, stdout_pipe, pipe_read, pipe_write);
	execute_command_internal(&cmd);
	destroy_command_internal(&cmd);
}

static void execute_command(cmd_node* node, bool stdin_pipe, bool stdout_pipe, int pipe_read, int pipe_write)
{
    if (node == NULL)
        return;
	if (node->type == NODE_CMDPATH)
		execute_simple_command(node, stdin_pipe, stdout_pipe, pipe_read, pipe_write);
}

static void execute_pipeline(cmd_node* tree)
{
    int file_desc[2];
	int p_write;
	int p_read;

	pipe(file_desc);
	p_write = file_desc[1];
    p_read = file_desc[0];
    execute_command(tree->left, false, true, 0, p_write);
    cmd_node* job = tree->right;
    while (job != NULL && job->type == NODE_PIPE)
    {
        close(p_write);
        pipe(file_desc);
        p_write = file_desc[1];
        execute_command(job->left, true, true, p_read, p_write);
        close(p_read);
        p_read = file_desc[0];
        job = job->right;
    }
    p_read = file_desc[0];
    close(p_write);
    execute_command(job, true, false, p_read, 0);
    close(p_read);
}

void executor(cmd_node* node)
{
	if (node == NULL)
        return ;
	if (node->type == NODE_PIPE)
		execute_pipeline(node);
	if (node->type == NODE_CMDPATH)
		execute_command(node, false, false, 0, 0);
}

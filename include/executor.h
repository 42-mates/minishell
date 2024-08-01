/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:53:12 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 14:34:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "parser.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>

typedef struct cmd_internal
{
	int     argc;
	char    **argv;
	bool    stdin_pipe;
	bool    stdout_pipe;
	int     pipe_read;
	int     pipe_write;
    char    *redirect_in;
    char    *redirect_out;
}   cmd_internal;

int		init_command_internal(cmd_node* node, cmd_internal* cmd, bool stdin_pipe, bool stdout_pipe, int pipe_read, int pipe_write, char *redirect_in, char *redirect_out);
void	execute_command_internal(cmd_internal* cmdinternal);
void	destroy_command_internal(cmd_internal* cmdinternal);
void	executor(cmd_node *tree);

#endif
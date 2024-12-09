/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:48 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/09 13:43:32 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect(int oldfd, int newfd, t_shell *shell)
{
	// (void)oldfd;
	// int old = 7755;
	if (dup2(oldfd, newfd) == -1)
	{
		//close(oldfd);
		close(newfd);
		//perror("dup2 123");
		shell->exit_status = 1;
		return ;
	}
	close(oldfd);
}

void	open_file(t_command *cmd, char *file, int flags, int newfd,
		t_shell *shell)
{
	int	oldfd;
	(void)cmd;	

	oldfd = open(file, flags, 0644);
	if (oldfd == -1)
	{
		perror(file); // test cat <"1""2""3""4""5" (busywaiting) exit ??
		shell->exit_status = 1;
	}
	if (cmd->name)
		redirect(oldfd, newfd, shell);
	return ;
}

void	set_redirection(t_command *cmd, t_shell *shell)
{
	if (cmd->input_file)
		open_file(cmd, cmd->input_file, O_RDONLY, STDIN_FILENO, shell);
	if (cmd->output_file)
		open_file(cmd, cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC,
			STDOUT_FILENO, shell);
	if (cmd->append_file)
		open_file(cmd, cmd->append_file, O_WRONLY | O_CREAT | O_APPEND,
			STDOUT_FILENO, shell);
	if (cmd->delimiter)
		heredoc(cmd->delimiter);
}

void	backup_original_fds(int *fds, t_shell *shell, t_pipe *pipeline)
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] == -1 || fds[1] == -1)
	{
		perror("dup");
		shell->exit_status = 1;
		free(pipeline);
		exit(EXIT_FAILURE);
	}
}

void	restore_original_fds(int *fds)
{
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
}

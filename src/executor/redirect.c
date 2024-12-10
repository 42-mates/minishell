/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:48 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/10 17:34:09 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect(int oldfd, int newfd, t_shell *shell)
{
	if (dup2(oldfd, newfd) == -1)
	{
		close(oldfd);
		// close(newfd);
		perror("dup2 in redirect()");
		shell->exit_status = 1;
		return (-1);
	}
	close(oldfd);
	return(0);
}

int	open_file(t_command *cmd, char *file, int flags, int newfd,
		t_shell *shell)
{
	int	oldfd;

	oldfd = open(file, flags, 0644);
	if (oldfd == -1)
	{
		perror(file);
		shell->exit_status = 1;
		return(-1);
	}
	if (cmd->name)
		redirect(oldfd, newfd, shell);
	close(oldfd);
	return (0);
}

int	set_redirection(t_command *cmd, t_shell *shell)
{
	if (cmd->input_file && !cmd->delimiter)
		if (open_file(cmd, cmd->input_file, O_RDONLY, STDIN_FILENO, shell) == -1)
			return (-1);
	if (cmd->output_file)
		if (open_file(cmd, cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC,
			STDOUT_FILENO, shell) == -1)
			return (-1);
	if (cmd->append_file)
		if (open_file(cmd, cmd->append_file, O_WRONLY | O_CREAT | O_APPEND,
			STDOUT_FILENO, shell) == -1)
			return (-1);
	return(0);
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

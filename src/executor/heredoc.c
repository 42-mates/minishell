/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/18 14:00:08 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document ", 2);
	ft_putstr_fd("delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
}

static void	write_input_to_fd(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			heredoc_warning(delimiter);
			break ;
		}
		if (ft_strcmp(delimiter, line) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static int	heredoc(char *delimiter)
{
	int		temp_fd;
	char	*temp;

	// int original_stdin;
	// original_stdin = dup(STDIN_FILENO);
	// if (original_stdin == -1)
	// {
	// 	perror("dup: failed to save STDIN_FILENO");
	// 	return (-1);
	// }
	if (!create_file(&temp, &temp_fd))
		return (-1);
	write_input_to_fd(temp_fd, delimiter);
	close(temp_fd);
	temp_fd = open(temp, O_RDONLY, 0644);
	if (temp_fd == -1)
	{
		perror("heredoc: open temporary file for read");
		free(temp);
		return (-1);
	}
	unlink(temp);
	free(temp);
	// if (dup2(original_stdin, STDIN_FILENO) == -1)
	// {
	// 	perror("dup2: failed to restore STDIN_FILENO");
	// 	close(original_stdin);
	// 	return (-1);
	// }
	// close(original_stdin);
	return (temp_fd);
}

// pipe version - testing
// int handle_heredoc(char *delimiter)
// {
// 	int pipe_fd[2];

// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	write_input_to_fd(pipe_fd[1], delimiter);
// 	close(pipe_fd[1]); // close write end
// 	return (pipe_fd[0]); // return read end
// }

static int	redirect_heredoc(int heredoc_fd)
{
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(heredoc_fd);
		return (-1);
	}
	close(heredoc_fd);
	return (0);
}

int	handle_heredocs(t_command *cmd)
{
	t_redirect	*r;
	int			heredoc_fd;

	heredoc_fd = -1;
	r = cmd->redirects;
	while (r)
	{
		if (r->type == R_HEREDOC)
		{
			if (heredoc_fd != -1)
				close(heredoc_fd);
			heredoc_fd = heredoc(r->filename);
			if (heredoc_fd == -1)
				return (-1);
		}
		r = r->next;
	}
	if (heredoc_fd != -1 && cmd->name)
		return (redirect_heredoc(heredoc_fd));
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/19 16:28:07 by oprosvir         ###   ########.fr       */
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

	if (!create_file(&temp, &temp_fd))
		return (-1);
	write_input_to_fd(temp_fd, delimiter);
	close(temp_fd);
	temp_fd = open(temp, O_RDONLY, 0644);
	if (temp_fd == -1)
	{
		perror("heredoc: open temp file for read");
		unlink(temp);
		free(temp);
		return (-1);
	}
	unlink(temp);
	free(temp);
	return (temp_fd);
}

int	cmd_heredocs(t_command *cmd)
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
	if (heredoc_fd != -1)
		cmd->heredoc_fd = heredoc_fd;
	return (0);
}

int	pipeline_heredocs(t_command *cmds, t_shell *shell)
{
	t_command	*cur;

	cur = cmds;
	while (cur)
	{
		if (cmd_heredocs(cur) == -1)
		{
			shell->exit_status = 1;
			return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

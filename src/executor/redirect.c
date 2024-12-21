/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:48 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/20 10:28:02 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_newfd(t_token_type type)
{
	if (type == R_OUTPUT || type == R_APPEND)
		return (STDOUT_FILENO);
	else
		return (STDIN_FILENO);
}

static int	get_flags(t_token_type type)
{
	if (type == R_INPUT)
		return (O_RDONLY);
	else if (type == R_OUTPUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else
		return (O_WRONLY | O_CREAT | O_APPEND);
}

static int	open_file(t_command *cmd, t_redirect *r, int flags)
{
	int	oldfd;
	int	newfd;

	oldfd = open(r->filename, flags, 0644);
	if (oldfd == -1)
		return (cmd_err(r->filename, NULL, strerror(errno), -1));
	if (cmd->name)
	{
		newfd = get_newfd(r->type);
		if (dup2(oldfd, newfd) == -1)
		{
			close(oldfd);
			perror("dup2 in open_file()");
			return (-1);
		}
		close(oldfd);
	}
	return (0);
}

int	set_redirection(t_command *cmd)
{
	t_redirect	*r;
	int			flags;

	r = cmd->redirects;
	while (r)
	{
		if (r->type != R_HEREDOC)
		{
			flags = get_flags(r->type);
			if (open_file(cmd, r, flags) == -1)
				return (-1);
		}
		r = r->next;
	}
	return (0);
}

void	case_redirects(t_command *cmd, t_shell *shell)
{
	if (cmd_heredocs(cmd) == -1)
	{
		shell->exit_status = 1;
		return ;
	}
	if (set_redirection(cmd) == -1)
	{
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = 0;
}

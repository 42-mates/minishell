/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:48 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/14 19:06:01 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect(int oldfd, int newfd, t_shell *shell)
{
	if (dup2(oldfd, newfd) == -1)
	{
		close(oldfd);
		perror("dup2 in redirect()");
		shell->exit_status = 1;
		return (-1);
	}
	close(oldfd);
	return (0);
}

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


int	set_redirection(t_command *cmd, t_shell *shell)
{
	t_redirect	*r;
	int			flags;
	int			oldfd;
	int			newfd;

	// if (cmd->redirects->type == R_HEREDOC)
	// {
	// 	if (heredoc(cmd, shell) == -1)
	// 		return (-1);
	// }	    
	r = cmd->redirects;
	while (r)
	{
        flags = get_flags(r->type);
        oldfd = open(r->filename, flags, 0644);
        if (oldfd == -1)
        {
            perror("open");
            shell->exit_status = 1;
            return (-1);
        }
        if (cmd->name)
        {
            newfd = get_newfd(r->type);
            if (redirect(oldfd, newfd, shell) == -1)
                return (-1);
        }
		r = r->next;
	}
	return (0);
}

// int	open_file(t_command *cmd, char *file, int flags, int newfd, 	t_shell *shell)
// {
// 	int	oldfd;

// 	oldfd = open(file, flags, 0644);
// 	if (oldfd == -1)
// 	{
// 		perror(file);
// 		shell->exit_status = 1;
// 		return (-1);
// 	}
// 	if (cmd->name)
// 		redirect(oldfd, newfd, shell);
// 	close(oldfd);
// 	return (0);
// }

// int	set_redirection(t_command *cmd, t_shell *shell)
// {
// 	if (cmd->input_file && !cmd->delimiter)
// 		if (open_file(cmd, cmd->input_file, O_RDONLY, STDIN_FILENO, shell) == -1)
// 			return (-1);
// 	if (cmd->output_file)
// 		if (open_file(cmd, cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC,
// 			STDOUT_FILENO, shell) == -1)
// 			return (-1);
// 	if (cmd->append_file)
// 		if (open_file(cmd, cmd->append_file, O_WRONLY | O_CREAT | O_APPEND,
// 			STDOUT_FILENO, shell) == -1)
// 			return (-1);
// 	return(0);
// }

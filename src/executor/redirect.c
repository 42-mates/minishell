/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:48 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/04 16:42:46 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_redirect(char *file, int flags, int newfd, t_shell *shell)
{
	int	oldfd;

	oldfd = open(file, flags, 0644);
	if (oldfd == -1)
	{
		perror("open file for redirection");
        shell->exit_status = 1;
        return ;
	}
	if (dup2(oldfd, newfd) == -1)
    {
        close(oldfd);
        perror("dup2");
        shell->exit_status = 1;
        return ;
    }
	close(oldfd);
}

void    set_redirection(t_command *cmd, t_shell *shell)
{
	if (cmd->input_file)
		open_redirect(cmd->input_file, O_RDONLY, STDIN_FILENO, shell);
	if (cmd->output_file)
		open_redirect(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO, shell);
	if (cmd->append_file)
		open_redirect(cmd->append_file, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO, shell);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:48 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/08 23:40:04 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_redirect(char *file, int flags, int newfd, t_shell *shell)
{
	int	oldfd;

	oldfd = open(file, flags, 0644);
	if (oldfd == -1)
	{
		perror("open file for redirection"); // test cat <"1""2""3""4""5" (busywaiting) exit ??
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/10 17:28:22 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_tmp_file(char *file, t_shell *shell)
{
    int fd;
    
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open temporary file for write");
        shell->exit_status = 1;
    }
    return (fd);
}

void write_input_to_file(int fd, char *delimiter)
{
    char    *line;

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            printf("minishell: EOF encountered before delimiter '%s'\n", delimiter);
            // close(fd);
            break;
        }
        if (ft_strcmp(delimiter, line) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
}

void    heredoc(t_command *cmd, t_shell *shell)
{
    int temp_fd;

    cmd->tmp_file_path = NULL;
    cmd->tmp_file_path = "temp_file";
    temp_fd = create_tmp_file(cmd->tmp_file_path, shell);
    if (temp_fd == -1)
        return;
    // printf("temporary file created\n");
    write_input_to_file(temp_fd, cmd->delimiter);
    // printf("user input written to temp file successfully\n");
    close(temp_fd);
    temp_fd = open(cmd->tmp_file_path, O_RDONLY, 0644);
    if (temp_fd == -1)
    {
        perror("heredoc: open temporary file for read");
        return ;
    }
    // printf("opened temp file for reading as input to command\n");
    if (dup2(temp_fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        shell->exit_status = 1;
        close(temp_fd);
        return;
    }
    // printf("successfully redirected file descriptor of temp file to read end\n");
    
    // if (cmd->input_file || cmd->output_file || cmd->append_file)
    //     set_redirection(cmd, shell);
    close(temp_fd);
}

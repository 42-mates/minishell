/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/09 11:41:32 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_tmp_file(char *file)
{
    int fd;
    
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND);
    if (fd == -1)
    {
        perror("open temporary file for write");
        exit(EXIT_FAILURE);
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

void    heredoc(char *delimiter)
{
    int fd;
    char    *file = "temp_file";

    fd = create_tmp_file(file);
    write_input_to_file(fd, delimiter);
    fd = open(file, O_RDONLY, 0644);
    if (fd == -1)
    {
        perror("open temporary file for read");
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
    }
    close(fd);
}
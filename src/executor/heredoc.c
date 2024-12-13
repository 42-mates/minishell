/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/13 20:11:11 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *convert_random_to_hex(unsigned char *random_bytes)
{
    char            *random_hex;
    const char      *hex_chars;
    int i;
    
    hex_chars = "0123456789abcdef";
    random_hex = malloc(sizeof(char) * RANDOM_BYTES * 2 + 1);
    if (!random_hex)
    {
        perror("malloc");
        return (NULL);
    }
    i = 0;
    while (i < RANDOM_BYTES)
    {
        random_hex[i * 2] = hex_chars[(random_bytes[i] >> 4) & 0xF];
        random_hex[i * 2 + 1] = hex_chars[random_bytes[i] & 0xF];
        i++;
    }
    random_hex[RANDOM_BYTES * 2] = '\0';
    return (random_hex);
}

char    *read_random_bytes()
{
    unsigned char   random_bytes[RANDOM_BYTES];
    int             urandom_fd;
    char            *random_hex;

    urandom_fd = open("/dev/urandom", O_RDONLY);
    if (urandom_fd == -1)
    {
        perror("open /dev/urandom");
        return (NULL);
    }
    if (read(urandom_fd, random_bytes, RANDOM_BYTES) != RANDOM_BYTES)
    {
        perror("read /dev/urandom");
        close(urandom_fd);
        return (NULL);
    }
    close(urandom_fd);
    random_hex = convert_random_to_hex(random_bytes);
    return (random_hex);
}

char    *random_file_name()
{
    char    *random_hex;
    char    *file_name;
    char    *prefix;

    prefix = "/tmp/minishell_heredoc_";
    random_hex = read_random_bytes();
    if (!random_hex)
        return (NULL);
    file_name = ft_strjoin(prefix, random_hex);
    printf("file_name = %s\n", file_name);
    return (file_name);
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

int create_tmp_file(char *file, t_shell *shell)
{
    int     fd;
    // char    *line;
    
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open temporary file for write");
        shell->exit_status = 1;
    }
    return (fd);
}

int heredoc(t_command *cmd, t_shell *shell)
{
    int     temp_fd;
    char    *tmp_file_path;

    tmp_file_path = random_file_name(); // create a temporary file path
    if (!tmp_file_path)
        return (-1);

    temp_fd = create_tmp_file(tmp_file_path, shell);
    write_input_to_file(temp_fd, cmd->delimiter);

    close(temp_fd);
    temp_fd = open(tmp_file_path, O_RDONLY, 0644);
    if (temp_fd == -1)
    {
        perror("heredoc: open temporary file for read");
        return (-1);
    }
    if (dup2(temp_fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        shell->exit_status = 1;
        close(temp_fd);
        return (-1);
    }
    close(temp_fd);
    if (tmp_file_path)
    	unlink(tmp_file_path);
    free(tmp_file_path);
    return (0);
}

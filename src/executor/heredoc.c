/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/13 21:00:08 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define RANDOM_BYTES 8

// int	ft_putnbr_base(unsigned long long n, char *base)
// {
// 	unsigned int	size;
// 	int				printed_chars;

// 	size = ft_strlen(base);
// 	printed_chars = 0;
// 	if (n >= size)
// 		printed_chars += ft_putnbr_base(n / size, base);
// 	printed_chars += ft_putchar(base[n % size]);
// 	return (printed_chars);
// }






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

char    *convert_random_to_hex(unsigned char *random_bytes)
{
    char            *random_hex;
    const char      *hex_chars = "0123456789abcdef";
    int i;
    
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

char    *create_random_file()
{
    char    *random_hex;
    char    *file_name;
    char    *prefix;

    prefix = "/tmp/minishell_heredoc";
    random_hex = read_random_bytes();
    file_name = ft_strjoin(prefix, random_hex);
    printf("file_name = %s\n", file_name);
    return (file_name);
}

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

int    heredoc(t_command *cmd, t_shell *shell)
{
    int temp_fd;
    char *temp;

    temp = create_random_file();
    // if (!cmd->tmp_file_path)
    //     return (-1);
    temp_fd = create_tmp_file(temp, shell);
    write_input_to_file(temp_fd, cmd->redirects->filename);
    close(temp_fd);
    temp_fd = open(temp, O_RDONLY, 0644);
    if (temp_fd == -1)
    {
        perror("heredoc: open temporary file for read");
        return -1;
    }
    if (dup2(temp_fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        shell->exit_status = 1;
        close(temp_fd);
        return -1;
    }
    close(temp_fd);
    if (temp)
		unlink(temp);
    return 0;
}

// int handle_heredoc(const char *delimiter)
// {
//     char *line;
//     int pipe_fd[2];

//     if (pipe(pipe_fd) == -1)
//     {
//         perror("pipe");
//         return -1;
//     }

//     while (1)
//     {
//         ft_putstr_fd("> ", 1); // Вывод приглашения
//         line = readline(NULL); // Считываем строку
//         if (!line || ft_strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break; // Завершаем, если достигли делимитера
//         }
//         ft_putstr_fd(line, pipe_fd[1]); // Пишем в пайп
//         ft_putstr_fd("\n", pipe_fd[1]);
//         free(line);
//     }
//     close(pipe_fd[1]); // Закрываем запись
//     return pipe_fd[0]; // Возвращаем дескриптор для чтения
// }


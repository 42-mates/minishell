/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:12:32 by mglikenf          #+#    #+#             */
/*   Updated: 2024/12/16 23:00:47 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document ", 2);
	ft_putstr_fd("delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
}

void write_input_to_fd(int fd, char *delimiter)
{
	char    *line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			heredoc_warning(delimiter);
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

/**
 * Converts an array of bytes into a hexadecimal string.
 * Each byte (8 bits) is split into two nibbles (4 bits each),
 * and converted into two characters.
 * The resulting string is null-terminated.
 */
// TODO : rename hexify_bytes
char	*convert_random_to_hex(unsigned char *random_bytes)
{
	int			i;
	char		*random_hex;
	const char	*hex_chars;

	hex_chars = "0123456789abcdef";
	random_hex = malloc(sizeof(char) * BYTES * 2 + 1);
	if (!random_hex)
		return (NULL);
	i = 0;
	while (i < BYTES)
	{
		random_hex[i * 2] = hex_chars[(random_bytes[i] >> 4) & 0xF];
		random_hex[i * 2 + 1] = hex_chars[random_bytes[i] & 0xF];
		i++;
	}
	random_hex[BYTES * 2] = '\0';
	return (random_hex);
}

/**
 * /dev/urandom is a built-in feature of the Linux OS,
 * not a third—party library or program. Its usage
 * complies with the Norm and subject rules.
 */
// TODO : rename get_random_hex
char    *read_random_bytes()
{
	unsigned char   random_bytes[BYTES];
	int             urandom_fd;
	char            *random_hex;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		return (NULL);
	if (read(urandom_fd, random_bytes, BYTES) != BYTES)
	{
		close(urandom_fd);
		return (NULL);
	}
	close(urandom_fd);
	random_hex = convert_random_to_hex(random_bytes);
	return (random_hex);
}

// TODO : rename generate_filename
char    *create_random_file()
{
	char    *random_hex;
	char    *filename;

	random_hex = read_random_bytes();
	if (!random_hex)
	{
		perror("failed to generate random bytes");
		return (NULL);
	}
	filename = ft_strjoin(TMP_DIR, random_hex);
	free(random_hex);
	if (!filename)
	{
		perror("failed to create file name");
		return (NULL);
	}
	//printf("file_name = %s\n", file_name);
	return (filename);
}

// TODO : rename open_temp_fd
int create_tmp_file(char *file)
{
	int	fd;
	
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("failed to open temp file");
		return (-1);
	}
	return (fd);
}

bool	create_file(char **temp, int *temp_fd)
{
	*temp = create_random_file();
	if (!temp)
		return (false);
	*temp_fd = create_tmp_file(*temp);
	if (*temp_fd == -1)
	{
		free(*temp);
		return (false);
	}
	return (true);
}

int	heredoc(char *delimiter)
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
        return -1;
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
int handle_heredoc(char *delimiter)
{
	int pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return -1;
	}
	write_input_to_fd(pipe_fd[1], delimiter);
	close(pipe_fd[1]); // close write end
	return pipe_fd[0]; // return read end
}

// TODO : refactor, heredoc w/pipes not working
 int handle_heredocs(t_command *cmd)
{
	t_redirect	*r;
	int heredoc_fd;

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
	{
		if (cmd->name)
		{
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
               	perror("dup2");
            	close(heredoc_fd);
                return (-1);
            }
			close(heredoc_fd);
		}
	}
	return (0);
}

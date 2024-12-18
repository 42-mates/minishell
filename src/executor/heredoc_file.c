/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:12:36 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/18 13:30:36 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Converts an array of bytes into a hexadecimal string.
 * Each byte (8 bits) is split into two nibbles (4 bits each),
 * and converted into two characters.
 * The resulting string is null-terminated.
 */
static char	*hexify_bytes(unsigned char *random_bytes)
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
static char	*get_random_hex(void)
{
	unsigned char	random_bytes[BYTES];
	int				urandom_fd;
	char			*random_hex;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		return (NULL);
	if (read(urandom_fd, random_bytes, BYTES) != BYTES)
	{
		close(urandom_fd);
		return (NULL);
	}
	close(urandom_fd);
	random_hex = hexify_bytes(random_bytes);
	return (random_hex);
}

static char	*generate_filename(void)
{
	char	*random_hex;
	char	*filename;

	random_hex = get_random_hex();
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
	return (filename);
}

static int	open_read_fd(char *file)
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
	*temp = generate_filename();
	if (!temp)
		return (false);
	*temp_fd = open_read_fd(*temp);
	if (*temp_fd == -1)
	{
		free(*temp);
		return (false);
	}
	return (true);
}

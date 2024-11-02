/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/02 14:55:40 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	display_prompt(void)
{
	char	*user;
	char	*cwd;

	user = getenv("USER");
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd");
		return ;
	}
	if (user)
		write(1, user, ft_strlen(user));
	else
		write(1, "user", 4);
	write(1, "@minishell:", 11);
	write(1, cwd, ft_strlen(cwd));
	write(1, "$ ", 2);
}

int	main(void)
{
	char	*line;

	while (1)
	{
		display_prompt();
		line = readline(NULL);
		if (!line)
			break ;
		free(line);
	}
	return (0);
}

/*
int	main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **tokens;
	t_command *cmd;

	while (1)
	{
		ft_printf("> ");
		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			perror("getline");
			exit(EXIT_FAILURE);
		}
		tokens = lexer(line);
		if (tokens[0] == NULL)
			continue ;
		cmd = parser(tokens);
		executor(cmd);
		free(tokens);
		free(cmd);
	}
	free(line);
	return (0);
}*/
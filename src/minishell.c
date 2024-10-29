/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/10/29 02:20:04 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}
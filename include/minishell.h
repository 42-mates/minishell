/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:30:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/10/29 02:25:22 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h> // printf, perror
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // read, write, close
# include <sys/wait.h> // waitpid

# define TOKEN_DELIM " \t\r\n\a"

typedef struct s_command
{
	char	*name;
	char	**args;
}			t_command;

char		**lexer(char *line);
t_command	*parser(char **tokens);
void		executor(t_command *cmd);

#endif
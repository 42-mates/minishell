/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:30:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/04 09:50:28 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>    // printf, perror
# include <stdlib.h>   // malloc, free, exit
# include <sys/wait.h> // waitpid
# include <unistd.h>   // read, write, close

# define TOKEN_DELIM " \t\r\n\a"

typedef struct s_command
{
	char	*name;
	char	**args;
}			t_command;

void		welcome_message(void);
bool		is_empty_line(const char *line);

t_command	*parser(char *line);
void		executor(t_command *cmd);

#endif
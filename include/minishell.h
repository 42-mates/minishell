/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:30:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/05 21:34:28 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define TOKEN_DELIM " \t\r\n\a"

typedef struct s_command
{
	char	*name;
	char	**args;
}			t_command;

void		welcome_message(void);
void		setup_signals(void);
void    	handle_eof(void);
void		free_command(t_command *cmd);
void		print_command(t_command *cmd);
bool		is_empty_line(const char *line);

t_command	*parser(char *line);
void		executor(t_command *cmd);

#endif
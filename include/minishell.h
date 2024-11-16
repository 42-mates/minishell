/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:30:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/16 13:17:50 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

# define TOKEN_DELIM " \t\r\n\a"

typedef struct s_command
{
	char			*name;
	char			**args;
}					t_command;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	int				exit_status; // ($?)
	t_env			*env_vars;
}					t_shell;

void				welcome_message(void);
void				setup_signals(void);
void				handle_eof(t_shell *shell);
void				free_command(t_command *cmd);
void				execute_builtin(t_command *cmd, t_shell *shell);
int					free_shell(t_shell *shell);
void				print_command(t_command *cmd);   // debug
void				free_memory(char **ptr);
void				free_env(t_env *lst);
bool				is_empty_line(const char *line);
t_env				*init_env(char **envp);
char				**convert_to_array(t_env *env_list);
char				*ft_getenv(const char *name, char **envp);
char				*get_full_exec_path(t_command *cmd, char **envp);
int					is_builtin(const char *cmd_name);

t_command			*parser(char *line, t_shell *shell);
void				executor(t_command *cmd, t_shell *shell);
void    			ft_exit(t_command *cmd, t_shell *shell);
void    			ft_pwd(t_shell *shell);
void    			ft_echo(t_command *cmd, t_shell *shell);
void				ft_env(t_command *cmd, t_shell *shell);

#endif
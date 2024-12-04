/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:30:58 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/04 13:38:55 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_command
{
	char				*name;
	char				**args;
	char				*output_file;
	char				*input_file;
	char				*append_file;
	char				*delimiter;
	struct s_command	*next;
}						t_command;

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_shell
{
	int                 exit_status;
	t_env				*env_vars;
}						t_shell;

// init
void					welcome_message(void);
void					setup_signals(void);
void					handle_eof(t_shell *shell);
t_env					*init_env(char **envp);
t_shell					*init_shell(int argc, char **argv, char **envp);

// executor & builtins
int						is_builtin(const char *cmd_name);
void					execute_builtin(t_command *cmd, t_shell *shell);
void					executor(t_command *cmd, t_shell *shell);
void					ft_exit(t_command *cmd, t_shell *shell);
void					ft_pwd(t_shell *shell);
void					ft_echo(t_command *cmd, t_shell *shell);
void					ft_env(t_command *cmd, t_shell *shell);
void					ft_unset(t_command *cmd, t_shell *shell);
int						ft_export(t_command *cmd, t_shell *shell);
int						ft_cd(t_command *cmd, t_shell *shell);

// lexer & parser
t_token					*lexer(char *line, t_shell *shell);
t_token                 *process_quotes(char *line, int *i, t_shell *shell, t_token *tokens);
t_token                 *process_variable(char *line, int *i, t_shell *shell, t_token *tokens);
t_token                 *process_meta(t_token *tokens, char *line, int *i);
t_token                 *process_word(char *line, int *i, t_shell *shell, t_token *tokens);
t_command				*parser(char *line, t_shell *shell);
char					*get_path(char *cmd_name, t_env *env_list);
char					*double_quote(char *line, int *i, t_shell *shell);
char					*single_quote(char *line, int *i);
char					*extract_var(char *line, int *i, t_shell *shell);
char					*extract_word(char *line, int *i, t_shell *shell);
char	                *ft_strjoin_char(char *str, char c);
char                    *expand_var(char *line, int *i, t_shell *shell, char *value);
char                    *add_char(char *line, int *i, char *value);
bool					is_meta(char c);
int						parse_redirects(t_token **tokens, t_command *cmd, t_shell *shell);
void					parse_args(t_token **tokens, t_command *cmd);
int						parse_pipe(t_token **tokens, t_command **current, t_shell *shell);
t_command				*init_command(t_shell *shell);
void					*err_msg(char *cmd, char *msg, t_shell *shell, int exit_status);
int						cmd_err(char *cmd, char *arg, char *msg, int err_num);

// utils
bool					is_empty_line(const char *line);
char					**convert_to_array(t_env *env_list);
char					*getenv_lst(const char *name, t_env *env_list);
void					setenv_lst(const char *name, const char *value, t_env **env_vars);
void					remove_var(t_env **env_list, const char *name);
void					*set_status(t_shell *shell, int status);
char					**append_to_array(char **array, const char *new_elem);
void					exec_error(const char *cmd, t_shell *shell);

// free
void					free_memory(char **ptr);
void					free_env(t_env *lst);
void					free_tokens(t_token *tokens);
void					free_commands(t_command *cmd);
int						free_shell(t_shell *shell);

// debug
void	                print_command(t_command *cmd);
void					print_tokens(t_token *tokens);

#endif
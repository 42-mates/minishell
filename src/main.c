/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/17 23:37:52 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*generate_prompt(t_env *env_list)
{
	char	*user;
	char	*cwd;
	char	*prompt;
	size_t	len_prompt;

	user = getenv_lst("USER", env_list);
	if (!user)
		user = "user";
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup(".");
	len_prompt = ft_strlen(user) + ft_strlen(cwd) + 14;
	prompt = malloc(len_prompt);
	if (!prompt)
	{
		free(cwd);
		return (NULL);
	}
	ft_strlcpy(prompt, user, len_prompt);
	ft_strlcat(prompt, "@minishell:", len_prompt);
	ft_strlcat(prompt, cwd, len_prompt);
	ft_strlcat(prompt, "$ ", len_prompt);
	free(cwd);
	return (prompt);
}

// isatty added, run additioinal tests
static char	*get_input(t_shell *shell)
{
	char	*prompt;
	char	*input;

	prompt = generate_prompt(shell->env_vars);
	if (!prompt)
		prompt = ft_strdup("minishell$ ");
	input = readline(prompt);
	free(prompt);
	if (!input)
		handle_eof(shell);
	// {
	// 	if (isatty(STDIN_FILENO))
	// 		handle_eof(shell);
	// 	return (NULL);
	// }
	return (input);
}

static void	minishell(char *line, t_shell *shell)
{
	t_command	*cmd;

	cmd = parser(line, shell);
	if (!cmd)
		return ;
	// print_command(cmd);	
	if (!cmd->next && is_builtin(cmd->name))
		case_builtin(cmd, shell);
	else if (!cmd->next && !cmd->name && cmd->redirects)
		case_redirects(cmd, shell);
	else
	{
		if (set_pipeline(cmd, shell))
			executor(cmd, shell, &shell->pipeline);
		cleanup_pipeline(&shell->pipeline);
	}
	free_commands(cmd);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_shell	*shell;

	shell = init_shell(argc, argv, envp);
	if (!shell)
		return (EXIT_FAILURE);
	while (1)
	{
		setup_signals();
		input = get_input(shell);
		if (!input)
			continue ;
		if (!is_empty_line(input))
			add_history(input);
		handle_signal(shell);
		minishell(input, shell);
		free(input);
	}
	return (SUCCESS);
}

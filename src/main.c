/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/10 23:47:43 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*generate_prompt(void)
{
	char	*user;
	char	*cwd;
	char	*prompt;
	size_t	len_prompt;

	user = getenv("USER");
	if (!user)
		user = "user";
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup("?");
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

static t_shell *init_shell(int argc, char **argv, char **envp)
{
	t_shell *shell;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Error: This program does not accept arguments.\n", 2);
		return (NULL);
	}
	setup_signals();
	welcome_message();
	shell = malloc(sizeof(t_shell));
	if (!shell)
        return (NULL);
	shell->env_vars = init_env(envp);
	// print_env_list(env_list);
	// add : обработка ошибок env
	if (!shell->env_vars)
    {
        free(shell);
        return (NULL);
    }
	shell->exit_status = 0;
	// add : обработка уровней вложенности shell
    return (shell);
}

static void	minishell(char *line, t_shell *shell)
{
	t_command	*cmd;

	cmd = parser(line);
	// print_command(cmd);
	executor(cmd, shell);
	free_command(cmd);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	char	*prompt;
	t_shell *shell;

	if ((shell = init_shell(argc, argv, envp)) == NULL)
		return (EXIT_FAILURE);
	while (1)
	{
		prompt = generate_prompt();
		if (!prompt)
			return (EXIT_FAILURE);
		input = readline(prompt);
		free(prompt);
		if (!input)
			handle_eof(shell);
		if (!is_empty_line(input))
			add_history(input);
		minishell(input, shell);
		free(input);
	}
	free_shell(shell);
	return (EXIT_SUCCESS);
}

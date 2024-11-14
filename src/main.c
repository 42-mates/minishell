/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/14 18:31:38 by oprosvir         ###   ########.fr       */
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

static char *get_input(t_shell *shell)
{
    char *prompt;
    char *input;

    prompt = generate_prompt();
	if (!prompt)
	{
		shell->exit_status = 1;
		exit(free_shell(shell));
	}
    input = readline(prompt);
    free(prompt);
    if (!input)
        handle_eof(shell);
    return (input);
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
	t_shell *shell;

	if ((shell = init_shell(argc, argv, envp)) == NULL)
		return (EXIT_FAILURE);
	while (1)
	{
		input = get_input(shell);
		if (!input)
			continue ;
		if (!is_empty_line(input))
			add_history(input);
		minishell(input, shell);
		free(input);
	}
	return free_shell(shell);
}

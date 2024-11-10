/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/10 21:20:15 by oprosvir         ###   ########.fr       */
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

static t_env *init_shell(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Error: This program does not accept arguments.\n", 2);
		return (NULL);
	}
	setup_signals();
	welcome_message();
	env_list = init_env(envp);
	// print_env_list(env_list);
	if (!env_list)
    {
        ft_putstr_fd("Error: Failed to initialize environment.\n", 2);
        return (NULL);
    }
    return (env_list);
	// add : обработка уровней вложенности shell
	// add : корр. очистка списка пер.окр.
	// add : обработка ошибок env
}

static void	minishell(char *line, t_env *env_list)
{
	t_command	*cmd;

	cmd = parser(line);
	// print_command(cmd);
	executor(cmd, env_list);
	free_command(cmd);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	char	*prompt;
	t_env	*env_list;

	if ((env_list = init_shell(argc, argv, envp)) == NULL)
		return (EXIT_FAILURE);
	while (1)
	{
		prompt = generate_prompt();
		if (!prompt)
			return (EXIT_FAILURE);
		input = readline(prompt);
		free(prompt);
		if (!input)
			handle_eof();
		if (!is_empty_line(input))
			add_history(input);
		minishell(input, env_list);
		free(input);
	}
	free_env(env_list);
	return (EXIT_SUCCESS);
}

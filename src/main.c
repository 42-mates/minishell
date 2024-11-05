/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:40:34 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/05 20:52:02 by oprosvir         ###   ########.fr       */
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

void free_command(t_command *cmd)
{
    int i = 0;

    if (!cmd)
        return;
    while (cmd->args[i])
    {
        free(cmd->args[i]);
        i++;
    }
    free(cmd->args);
    free(cmd);
}

// for debugging
void print_command(t_command *cmd)
{
    int i;

    if (!cmd)
    {
        printf("Command is NULL\n");
        return;
    }
    printf("Command name: %s\n", cmd->name);
    printf("Arguments:\n");
    if (cmd->args)
    {
        i = 0;
        while (cmd->args[i])
        {
            printf("  args[%d]: %s\n", i, cmd->args[i]);
            i++;
        }
    }
    else
        printf("  No arguments\n");
}

static int	init_shell(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Error: This program does not accept arguments.\n", 2);
		return (1);
	}
	setup_signals();
	// add : копирование переменных окружения
	// add : обработка уровней вложенности shell
	welcome_message();
	return (0);
}

static void	minishell(char *line)
{
	t_command *cmd;

	cmd = parser(line);
	print_command(cmd);
	//executor(cmd);
	free_command(cmd);	
}

int	main(int argc, char *argv[])
{
	char	*line;
	char	*prompt;

	if (init_shell(argc, argv) != 0)
		return (EXIT_FAILURE);
	while (1)
	{
		prompt = generate_prompt();
		if (!prompt)
			return (EXIT_FAILURE);
		line = readline(prompt);
		free(prompt);
		if (!line)
			handle_eof();
		if (!is_empty_line(line))
			add_history(line);
		minishell(line);
		free(line);
	}
	return (EXIT_SUCCESS);
}

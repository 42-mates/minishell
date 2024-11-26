/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/25 23:38:03 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void handle_redirections(char *command)
{
    // Parse and apply <, >, <<, and >> redirections
}

char *expand_env_vars(char *input)
{
    // Parse and replace environment variables (e.g., $USER)
}

void handle_pipes(char **commands)
{
    // Create a pipeline if commands contain `|`
}

typedef struct s_command {
    char *name;         // Имя команды или путь к исполняемому файлу
    char **args;        // Аргументы команды
    char *input_file;   // Файл для ввода (<)
    char *output_file;  // Файл для вывода (>)
    char *append_file;  // Файл для добавления (>>)
    char *delimiter;    // Делимитер для << (here-document)
    struct s_command *next; // Указатель на следующую команду в пайплайне
} t_command;

 char **tokens = split_input_into_tokens(input);
cmd->command = find_command_path(tokens[0]); // Ищем путь к "ls"
    cmd->args = tokens;                          // Аргументы — это все токены
    

t_command *parser(char *line, t_shell *shell) {
    t_command *head = NULL;
    t_command *current = NULL;
    char **tokens = tokenize(line); // Разделяем строку на токены с учётом кавычек и редиректов

    for (int i = 0; tokens[i]; i++) {
        if (is_pipe(tokens[i])) {
            // Учитывай пайп, чтобы создать новую команду
            current = add_new_command(&head, current);
        } else if (is_redirect(tokens[i])) {
            // Обработка редиректов
            handle_redirect(current, tokens, &i);
        } else {
            // Добавляем аргумент или имя команды
            add_argument(current, tokens[i]);
        }
    }
    free(tokens);
    return head;
}

*/

t_command *init_command(void)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        ft_printf("allocation error\n");
        return (NULL);
    }
    cmd->name = NULL;
    cmd->args = NULL;
    cmd->output_file = NULL;
    cmd->input_file = NULL;
    cmd->next = NULL;
    return cmd;
}

// рабочая
/*
t_command *parser(char *line, t_shell *shell)
{
    char **commands;
    t_command *head = NULL;
    t_command *current = NULL;
    int i = 0;

    commands = ft_split(line, '|'); // Разделяем команды по '|'
    while (commands[i])
    {
        t_command *new_cmd = init_command();
        new_cmd->args = ft_strtok(commands[i], TOKEN_DELIM);
        new_cmd->name = new_cmd->args[0];

        if (!is_builtin(new_cmd->name))
        {
            char *exec_path = get_path(new_cmd, shell->env_vars);
            if (!exec_path)
            {
                ft_putstr_fd(new_cmd->name, 2);
                ft_putstr_fd(": command not found\n", 2);
                shell->exit_status = 127;
                free(new_cmd);
                continue;
            }
            new_cmd->name = exec_path;
            new_cmd->args[0] = exec_path;
        }

        if (!head)
            head = new_cmd;
        else
            current->next = new_cmd;

        current = new_cmd;
        i++;
    }
    free_memory(commands);
    return head;
}*/

t_command *parser(char *line, t_shell *shell)
{
    t_token *tokens;
    t_command *head = NULL;
    
    tokens = lexer(line, shell);
    if (!tokens)
    {
        shell->exit_status = 1;
        return (NULL);
    }
    print_tokens(tokens);
    free_tokens(tokens);
    return (head);
}

/*
t_command	*parser(char *line, t_shell *shell)
{
	char		**tokens;
	t_command	*cmd;
	char		*exec_path;

	tokens = ft_strtok(line, TOKEN_DELIM);
	if (!tokens || !tokens[0])
	{
		free(tokens);
		return (NULL);
	}
	cmd = init_command();
	cmd->name = tokens[0];
	cmd->args = tokens;
	if (!is_builtin(cmd->name))
	{
		exec_path = get_path(cmd, shell->env_vars);
		if (!exec_path)
		{
			ft_putstr_fd(tokens[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			shell->exit_status = 127;
			free_memory(tokens);
			free(cmd);
			return (NULL);
		}
		cmd->name = exec_path;
		tokens[0] = exec_path;
		cmd->args = tokens;
	}
	return (cmd);
}*/

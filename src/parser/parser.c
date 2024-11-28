/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/28 20:16:32 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(t_shell *shell)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (set_status(shell, 1));
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->output_file = NULL;
	cmd->input_file = NULL;
    cmd->append_file = NULL;
    cmd->delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

bool	is_redirect(t_token_type type)
{
	return (type == REDIRECT_OUT ||
			type == REDIRECT_IN ||
			type == APPEND ||
			type == HEREDOC);
}

char	**append_to_array(char **array, const char *new_elem)
{
	int		i;
	int		size;
	char	**new_array;

	size = 0;
	while (array && array[size])
		size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array && array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_memory(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[i] = ft_strdup(new_elem);
	new_array[i + 1] = NULL;
	free_memory(array);
	return (new_array);
}

void	handle_args(t_token **tokens, t_command *cmd)
{
	if (!cmd->name)
	{
		cmd->name = ft_strdup((*tokens)->value);
		cmd->args = append_to_array(NULL, cmd->name);
	}
	else
		cmd->args = append_to_array(cmd->args, (*tokens)->value);
	(*tokens) = (*tokens)->next;
}

int	handle_redirects(t_token **tokens, t_command *cmd, t_shell *shell)
{
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
		return (err_msg(NULL, "syntax error near unexpected token", shell, 2), -1);
	if ((*tokens)->type == REDIRECT_OUT)
		cmd->output_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == APPEND)
		cmd->append_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == REDIRECT_IN)
		cmd->input_file = ft_strdup((*tokens)->next->value);
	else if ((*tokens)->type == HEREDOC)
		cmd->delimiter = ft_strdup((*tokens)->next->value);
	(*tokens) = (*tokens)->next->next;
	return (0);
}

static int handle_pipe(t_token **tokens, t_command **current, t_shell *shell)
{
    if (!(*current) || !(*tokens)->next || (*tokens)->next->type != WORD)
        return (err_msg(NULL, "syntax error near unexpected token `|`", shell, 2), -1);
    (*current)->next = init_command(shell);
    if (!(*current)->next)
        return (-1);
    *current = (*current)->next;
    *tokens = (*tokens)->next;
    return (0);
}

int process_token(t_token **tokens, t_command *current, t_shell *shell)
{
	char *exec_path;
	
    if (is_redirect((*tokens)->type))
    {
        if (handle_redirects(tokens, current, shell) == -1)
            return (-1);
    }
    else
    {
        handle_args(tokens, current);
        if (current->name && !is_builtin(current->name))
        {
            exec_path = get_path(current->name, shell->env_vars);
            if (!exec_path)
				return (err_msg(current->name, "command not found", shell, 127), -1);
            free(current->name);
            current->name = exec_path;
			free(current->args[0]);
            current->args[0] = ft_strdup(current->name);
        }
    }
    return (0);
}

t_command *parse_tokens(t_token *tokens, t_shell *shell)
{
    t_command *head = NULL;
    t_command *current = NULL;

    while (tokens)
    {
        if (tokens->type == PIPE)
        {
            if (handle_pipe(&tokens, &current, shell) == -1)
                return (free_commands(head), NULL);
        }
        else
        {
            if (!current)
            {
                current = init_command(shell);
                if (!current)
                    return (free_commands(head), NULL);
                if (!head)
                    head = current;
            }
            if (process_token(&tokens, current, shell) == -1)
                return (free_commands(head), NULL);
        }
    }
    return (head);
}

t_command *parser(char *line, t_shell *shell)
{
    t_token *tokens;
    t_command *head;

    tokens = lexer(line, shell);
    if (!tokens)
        return (set_status(shell, 1));
    head = parse_tokens(tokens, shell);
    free_tokens(tokens);
    return (head);
}

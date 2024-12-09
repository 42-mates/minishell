/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/09 21:23:41 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	current_token(t_token **tokens, t_command *current, t_shell *shell)
{
	char	*exec_path;

	if (is_redirect((*tokens)->type))
		return (parse_redirects(tokens, current, shell));
	parse_args(tokens, current);
	if (current->name && !is_builtin(current->name))
	{
		exec_path = get_path(current->name, shell->env_vars);
		if (!exec_path)
			return (err_msg(current->name, "command not found", shell, 127),
				-1);
		free(current->name);
		current->name = exec_path;
		free(current->args[0]);
		current->args[0] = ft_strdup(current->name);
	}
	return (0);
}

static t_command	*parse_command(t_token **tokens, t_shell *shell,
		t_command **current)
{
	if (!*current)
	{
		*current = init_command(shell);
		if (!*current)
			return (NULL);
	}
	if (current_token(tokens, *current, shell) == -1)
		return (NULL);
	return (*current);
}

static t_command	*build_command(t_token *tokens, t_shell *shell)
{
	t_command	*head;
	t_command	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (parse_pipe(&tokens, &current, shell) == -1)
				return (free_commands(head), NULL);
		}
		else
		{
			current = parse_command(&tokens, shell, &current);
			if (!current)
				return (free_commands(head), NULL);
			if (!head)
				head = current;
		}
	}
	return (head);
}

t_command	*parser(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_command	*head;

	tokens = lexer(line, shell);
	if (!tokens)
		return (NULL);
	// print_tokens(tokens);
	head = build_command(tokens, shell);
	free_tokens(tokens);
	return (head);
}

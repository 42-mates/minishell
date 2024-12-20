/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:47:23 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/09 01:28:13 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	unclosed_quotes(char *line)
{
	bool	single_q;
	bool	double_q;
	int		i;

	i = 0;
	single_q = false;
	double_q = false;
	while (line[i])
	{
		if (line[i] == '\'' && !double_q)
			single_q = !single_q;
		else if (line[i] == '"' && !single_q)
			double_q = !double_q;
		i++;
	}
	return (single_q || double_q);
}

t_token	*lexer(char *line, t_shell *shell)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	if (unclosed_quotes(line))
		return (err_msg(NULL, "syntax error: unmatched or unclosed quotes",
				shell, 2));
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (is_meta(line[i]))
			tokens = meta_token(tokens, line, &i);
		else
			tokens = word_token(line, &i, shell, tokens);
	}
	return (tokens);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:06:24 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 14:24:01 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include "libft.h"

enum TokenType{
	CHAR_PIPE		= '|',
	CHAR_GREATER	= '>',
	TOKEN			= -1,
};

# define TOKEN_DELIM " \t\n\r\a"

typedef struct s_token {
	char			*data;
	int				type;
	struct s_token	*next;
}	t_token;

char		**lexer(char *line);
t_token		*convert_from_tokens(char **tokens);
void		print_tokens(t_token *token);

#endif
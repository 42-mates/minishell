/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 00:21:57 by oprosvir          #+#    #+#             */
/*   Updated: 2023/06/11 00:21:57 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	get_word_count(char const *s, char c)
{
	int		count;
	int		in_substr;

	in_substr = 0;
	count = 0;
	while (*s != '\0')
	{
		if (*s == c)
			in_substr = 0;
		else if (in_substr == 0)
		{
			in_substr = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*make_word(char const *s, size_t len)
{
	char	*word;
	size_t	i;

	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	next_word(char const **s, char c, char **words, int i)
{
	int		j;

	if (**s != c)
	{
		j = 0;
		while ((*s)[j] != c && (*s)[j] != '\0')
			j++;
		words[i] = make_word(*s, j);
		if (!words[i])
			return (0);
		*s += j;
		return (1);
	}
	(*s)++;
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**words;
	int		word_count;
	int		i;

	if (!s)
		return (NULL);
	word_count = get_word_count(s, c);
	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		if (next_word(&s, c, words, i))
			i++;
	}
	words[i] = NULL;
	return (words);
}

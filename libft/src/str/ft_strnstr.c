/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 22:13:31 by oprosvir          #+#    #+#             */
/*   Updated: 2024/03/04 12:32:34 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;

	if (!haystack && len == 0)
	{
		return (NULL);
	}
	if (!*needle)
	{
		return ((char *)haystack);
	}
	needle_len = ft_strlen(needle);
	i = 0;
	while (haystack[i] && i + needle_len <= len)
	{
		if (!ft_memcmp(&haystack[i], needle, needle_len))
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}

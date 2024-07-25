/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 21:02:38 by oprosvir          #+#    #+#             */
/*   Updated: 2023/06/12 17:33:46 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*d;
	unsigned char	*s;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (d < s)
	{
		while (len-- > 0)
		{
			*d++ = *s++;
		}
	}
	else
	{
		d += len;
		s += len;
		while (len-- > 0)
		{
			*--d = *--s;
		}
	}
	return (dst);
}

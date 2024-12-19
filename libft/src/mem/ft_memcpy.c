/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 20:29:32 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/19 18:55:57 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	if (!dst || !src)
		return (NULL);
	if (n == 0)
		return (dst);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	while (n-- > 0)
		*d++ = *s++;
	return (dst);
}

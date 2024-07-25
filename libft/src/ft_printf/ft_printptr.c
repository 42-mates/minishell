/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:10:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/03/04 12:29:53 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_printhexp(unsigned long long num)
{
	char	*base_hex;
	int		count;

	count = 0;
	base_hex = "0123456789abcdef";
	if (num >= 16)
	{
		count += ft_printhexp(num / 16);
	}
	count += ft_printchar(base_hex[num % 16]);
	return (count);
}

int	ft_printptr(unsigned long long num)
{
	int	count;

	if (!num)
		return (ft_printstr("(nil)"));
	else
	{
		count = ft_printstr("0x");
		count += ft_printhexp(num);
		return (count);
	}
}

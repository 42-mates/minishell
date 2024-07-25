/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 12:43:54 by oprosvir          #+#    #+#             */
/*   Updated: 2024/03/04 12:29:38 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printhex(unsigned int num, bool isCapital)
{
	char	*base_hex;
	int		count;

	count = 0;
	if (isCapital)
		base_hex = "0123456789ABCDEF";
	else
		base_hex = "0123456789abcdef";
	if (num >= 16)
	{
		count += ft_printhex(num / 16, isCapital);
	}
	count += ft_printchar(base_hex[num % 16]);
	return (count);
}

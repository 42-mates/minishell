/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:24:13 by oprosvir          #+#    #+#             */
/*   Updated: 2024/03/04 12:29:20 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_unsigned(unsigned int n)
{
	char	c;
	int		count;

	count = 0;
	if (n > 9)
	{
		count += ft_print_unsigned(n / 10);
		count += ft_print_unsigned(n % 10);
	}
	else
	{
		c = n + '0';
		write(1, &c, 1);
		count++;
	}
	return (count);
}

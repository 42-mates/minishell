/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:48:28 by oprosvir          #+#    #+#             */
/*   Updated: 2024/03/04 12:29:29 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	convert(char c, va_list args)
{
	int	num;

	num = 0;
	if (c == 'c')
		num += ft_printchar(va_arg(args, int));
	else if (c == 's')
		num += ft_printstr(va_arg(args, char *));
	else if (c == 'i' || c == 'd')
		num += ft_printnum(va_arg(args, int));
	else if (c == 'u')
		num += ft_print_unsigned(va_arg(args, unsigned int));
	else if (c == 'x')
		num += ft_printhex(va_arg(args, unsigned int), false);
	else if (c == 'X')
		num += ft_printhex(va_arg(args, unsigned int), true);
	else if (c == 'p')
		num += ft_printptr(va_arg(args, unsigned long long));
	return (num);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	va_start(args, format);
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == '%')
				count += ft_printchar(format[i]);
			else
				count += convert(format[i], args);
		}
		else
			count += ft_printchar(format[i]);
		i++;
	}
	va_end(args);
	return (count);
}

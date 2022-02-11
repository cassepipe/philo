/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ulong_repr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 22:25:22 by tpouget           #+#    #+#             */
/*   Updated: 2022/02/11 22:25:22 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ulong_repr.h"

static unsigned long    to_the_power_of(unsigned long n, unsigned long p)
{
	unsigned long result;

	result = 1;
	while (p--)
		result *= n;
	return (result);
}

char    *ulong_repr(long n, char *buffer, int *len)
{
	unsigned long   i;
	unsigned long   digit;
	unsigned long   a_power_of_ten;
	int	            pos;

	pos = 1;
	i = n;
	while (i /= 10)
		pos++;
	*len += pos;
	a_power_of_ten = to_the_power_of(10, pos - 1);
	i = 0;
	while (--pos >= 0)
	{
		digit = n / a_power_of_ten;
		buffer[i] = "0123456789"[digit];
		n = n - digit * a_power_of_ten;
		a_power_of_ten /= 10;
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}

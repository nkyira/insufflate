/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodavis <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:27:38 by jodavis           #+#    #+#             */
/*   Updated: 2025/01/22 18:53:52 by jodavis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgnl.h"

void	reset_str(char *str)
{
	int	i;

	i = -1;
	while (++i <= BUFFER_SIZE)
		str[i] = '\0';
}

size_t	push_len(char *str_left)
{
	size_t	i;

	i = 0;
	while (str_left[i] && i < BUFFER_SIZE)
		i++;
	return (i);
}

int	contains_nl(char *str)
{
	while (*str)
	{
		if (*(str++) == '\n')
			return (1);
	}
	return (0);
}

void	copy_str(char *src, char *dest, int l)
{
	while (*src && l--)
		*(dest++) = *(src++);
}

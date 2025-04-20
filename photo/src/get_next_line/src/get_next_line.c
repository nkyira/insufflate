/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodavis <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:27:24 by jodavis           #+#    #+#             */
/*   Updated: 2025/01/22 20:00:44 by jodavis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgnl.h"
#include <stdio.h>
#include <fcntl.h>

static void	save_lft(char *str_lft, char *cursor, char *buff, ssize_t bytes_rd)
{
	if ((size_t)(cursor - buff))
	{
		bytes_rd--;
		str_lft[bytes_rd] = '\0';
		while (bytes_rd-- > 0)
			str_lft[bytes_rd] = cursor[bytes_rd];
	}
}

static char	*cut_str_lft(char *str_lft)
{
	char	*cursor;
	char	*line;
	size_t	offset;

	cursor = str_lft;
	while (*cursor != '\n')
		cursor++;
	offset = cursor - str_lft + 1;
	line = malloc(sizeof(char) * (offset + 1));
	if (!line)
		return (NULL);
	copy_str(str_lft, line, offset);
	line[offset] = '\0';
	copy_str(cursor + 1, str_lft, push_len(str_lft) - push_len(line));
	str_lft[push_len(str_lft) - push_len(line)] = '\0';
	return (line);
}

static char	*line_alloc(size_t line_l, size_t offset, char *cursor, size_t push)
{
	char	*str;

	if (!(line_l + offset + push))
		return (NULL);
	str = malloc(sizeof(char) * (line_l + offset + push + 1));
	if (!str)
		return (NULL);
	str += line_l + offset + push;
	*str = '\0';
	while (offset--)
		*(--str) = *(--cursor);
	str -= push;
	return (str);
}

static char	*my_read_next(int fd, ssize_t line_l, char *str_lft, size_t push)
{
	char	buff[BUFFER_SIZE];
	ssize_t	bytes_rd;
	char	*cursor;

	bytes_rd = read(fd, buff, BUFFER_SIZE);
	if (bytes_rd < 0)
		return (NULL);
	cursor = buff;
	if (!bytes_rd)
		return (line_alloc(line_l, (cursor - buff), cursor, push));
	while (*cursor != '\n' && --bytes_rd)
		cursor++;
	if (*cursor == '\n')
	{
		save_lft(str_lft, cursor + 1, buff, bytes_rd);
		return (line_alloc(line_l, (cursor - buff) + 1, cursor + 1, push));
	}
	bytes_rd = cursor - buff + 1;
	cursor = my_read_next(fd, line_l + bytes_rd, str_lft, push);
	if (!cursor)
		return (NULL);
	while (bytes_rd--)
		*(--cursor + push) = buff[bytes_rd];
	return (cursor);
}

char	*get_next_line(int fd)
{
	static char	str_lft[BUFFER_SIZE + 1];
	char		str_saved[BUFFER_SIZE + 1];
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
	{
		reset_str(str_lft);
		return (NULL);
	}
	reset_str(str_saved);
	if (contains_nl(str_lft))
		return (cut_str_lft(str_lft));
	else
		copy_str(str_lft, str_saved, BUFFER_SIZE + 1);
	reset_str(str_lft);
	line = my_read_next(fd, 0, str_lft, push_len(str_saved));
	if (line)
		copy_str(str_saved, line, BUFFER_SIZE + 1);
	return (line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:46:50 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/03 12:40:21 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	gnl_append_from_stream(t_line *line, t_stream *s, uint32_t count)
{
	uint32_t	i;

	if (!line->line)
		return ;
	i = 0;
	while (i < count)
	{
		line->line[line->index + i] = s->buffer[s->cursor + i];
		i++;
	}
	line->index += count;
	s->cursor += count;
	line->line[line->index] = 0;
}

int	gnl_is_eol(t_line line)
{
	if (!line.line)
		return (1);
	return (line.line[line.index - 1] == '\n');
}

uint32_t	gnl_chunk_size(t_stream s)
{
	ssize_t	i;

	i = 0;
	while (i < (s.bytes_read - (ssize_t)s.cursor))
	{	
		i++;
		if (s.buffer[s.cursor + i - 1] == '\n')
		{
			break ;
		}
	}
	return ((uint32_t)i);
}

/*
	Synopsis of get_next_line:
	----------------------------------
	line.line is initialized to (null).
	while (we haven't met the EOF)
	{
		we first look if there are characters left in our
		static buffer.
		If yes,
	 		we make sure our line structure is
			prepared to receive the extra characters and
			then we append them.
			if we have are at the end of line,
			we are finished and break.
		Now, we refill our buffer.
	}
	When EOF is reached, we reset the buffer.
	Return line.line
	------------------------------------

	About malloc failures:
	Malloc failures do not result to an immediate break
	out of the function.
	Instead, line.line is just set to (null) and the
	next functions that receive it just don't do anything.
	My gnl_is_eol() function detects (null) as being an end
	of line and makes it break out of our loop.
*/
char	*get_next_line(int fd)
{
	static t_stream	s = {.cursor = 0, .bytes_read = -42};
	t_line			line;
	uint32_t		count;

	line.line = 0;
	while (s.bytes_read && s.bytes_read != -1)
	{
		count = gnl_chunk_size(s);
		if (count)
		{
			gnl_init_if_necessary(&line);
			gnl_resize_if_necessary(&line, count);
			gnl_append_from_stream(&line, &s, count);
			if (gnl_is_eol(line))
				break ;
		}
		s.bytes_read = read(fd, s.buffer, BUFFER_SIZE);
		s.cursor = 0;
	}
	if (!s.bytes_read || s.bytes_read == -1)
		s.bytes_read = -42;
	return (line.line);
}

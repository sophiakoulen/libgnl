/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:46:46 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/03 12:40:34 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define INIT_LINE_SIZE 32

/*
	The t_stream structure is used to store the additional
	data needed for reading from our file descriptor.
	
	It has a stack-allocated *buffer* that contains the last characters read with
	the read() function. Since we will keep track of the number of characters
	read, it needn't be nul-terminated and therefore isn't a C-string.
	
	It has the integer bytes_read, the result of the last
	read() call. This means bytes_read can be -1, 0, or positive.
	It can also be -42, the value we initialize it to at the first
	function call and when we finished reading from the file.
	This is to make sure we enter the reading into buffer procedure
	the first time we enter the function.
	
	The cursor keeps track of our position in the buffer, since a
	buffer might contain multiple lines and we need to continue 
	looking after the last \n when we call the function next time.
*/
typedef struct s_stream
{
	char			buffer[BUFFER_SIZE];
	uint32_t		cursor;
	ssize_t			bytes_read;
}	t_stream;

/*
	The t_line structure represents our line.
	
	The line is the heap-allocated C-string that will be
	returned by our function.

	The index is the position the next character will be written.
	index + 1 will be a nul-character.
*/
typedef struct s_line
{
	char			*line;
	uint32_t		index;
}	t_line;

uint32_t		gnl_roundpow2(uint32_t n);
void			gnl_strncpy(char *dst, char *src, uint32_t n);
void			gnl_resize_line(t_line *line, uint32_t new_size);

void			gnl_resize_if_necessary(t_line *line, uint32_t count);
void			gnl_init_if_necessary(t_line *line);
void			gnl_append_from_stream(t_line *line,
					t_stream *s, uint32_t count);
int				gnl_is_eol(t_line line);
unsigned int	gnl_chunk_size(t_stream s);

char			*get_next_line(int fd);

#endif
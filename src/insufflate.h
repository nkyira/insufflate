#ifndef INSUFFLATE_H
# define INSUFFLATE_H
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include "get_next_line/src/libgnl.h"

typedef struct	s_data
{
	int	width;
	int	height;
	int	skip;
	char	*frame;
	int	frame_len;
}	t_data;

#endif

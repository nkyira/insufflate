#ifndef INSUFFLATE_H
# define INSUFFLATE_H
# ifndef F_WIDTH
#  define F_WIDTH 191
# endif
# ifndef F_HEIGHT
#  define F_HEIGHT 60
# endif
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include "get_next_line/src/libgnl.h"

typedef struct	s_frame_data
{
	int	width;
	int	height;
	int	skip;
	char	*frame;
	int	frame_len;
}	t_frame_data;

void	convert_image(char *path);
int	get_size(int imfd, int *w, int *h);
int	setup(char *path, t_frame_data **data);
int	pixel_to_ascii(t_frame_data *data, char *path);
int	make_ascii_image(t_frame_data *data);

#endif

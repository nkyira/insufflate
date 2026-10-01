#ifndef INSUFFLATE_H
# define INSUFFLATE_H
# ifndef F_WIDTH
#  define F_WIDTH 191
# endif
# ifndef F_HEIGHT
#  define F_HEIGHT 60
# endif
# include <unistd.h>
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include "get_next_line/src/libgnl.h"

typedef struct	s_data
{
	int		width;
	int		height;
	int		len;
	int		frame_count;
	char	**all_frames;
}	t_data;

/*					Utils					*/
void free_perror(char *str, char *s1, char *s2, char *s3);
void handle_error(char *str, int fd1, int fd2);
void init_data(t_data *data, int w, int h, int fc);
void print_data(t_data *data);
void ffprobe(char *input);
void ffmpeg(char *input, int width, int height);

/*					Converts video format into .ppm frames				*/
int get_skip(char *path);
void setup(t_data *data, char *path);

/*		Converting rgb values to charachers based on brightness			*/
char *pixel_to_ascii(t_data *data, char *path, int skip);

#endif

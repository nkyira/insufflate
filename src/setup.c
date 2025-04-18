#include "insufflate.h"

int get_size(int imfd, int *w, int *h)
{
	char	*width;
	char	*height;
	int	skip;
	char	*line;

	skip = 0;
	line = get_next_line(imfd);
	if (!line)
		return -1;
	skip += strlen(line);
	free(line);
	line = get_next_line(imfd);
	if (!line)
		return -1;
	width = strndup(line, strchr(line, ' ') - line);
	if (!width)
		return -1;
	height = line + strlen(width) + 1;
	height = strndup(height, strchr(height, '\n') - height);
	if (!height)
		return -1;
	*w = atoi(width);
	*h = atoi(height);
	skip += strlen(line);
	free(width);
	free(height);
	free(line);
	return skip;
	
}

int setup(char *path, t_frame_data **data)
{
	int	imfd;
	int	skip;
	int	w;
	int	h;
	char	*line;

	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return -1;
	}
	get_next_line(-1);
	skip = get_size(imfd, &w, &h);
	if (skip < 0)
	{
		close(imfd);
		return 1;
	}
	*data = malloc(sizeof(t_frame_data));
	if (!*data)
	{
		close(imfd);
		return 1;
	}
	(*data)->width = w;
	(*data)->height = h;
	(*data)->frame_len = (*data)->width * (*data)->height;
	printf("width = %d, height = %d, ", (*data)->width, (*data)->height);
	line = get_next_line(imfd);
	close(imfd);
	if (!line)
	{
		free(*data);
		return 1;
	}
	skip += strlen(line);
	(*data)->skip = skip;
	free(line);
	return 0;
}

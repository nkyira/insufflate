#include "insufflate.h"

static int setup(char *path, t_data **data)
{
	int	imfd;
	int	skip;
	char	*line;
	char	*width;
	char	*height;

	skip = 0;
	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return 1;
	}
	line = get_next_line(imfd);
	if (!line)
		return 1;
	skip += strlen(line);
	free(line);
	line = get_next_line(imfd);
	if (!line)
		return 1;
	width = strndup(line, strchr(line, ' ') - line);
	if (!width)
		return 1;
	height = line + strlen(width) + 1;
	height = strndup(height, strchr(height, '\n') - height);
	if (!height)
		return 1;
	*data = malloc(sizeof(t_data));
	if (!*data)
		return 1;
	(*data)->width = atoi(width);
	(*data)->height = atoi(height);
	(*data)->frame_len = (*data)->width * (*data)->height * 3;
	printf("width = %d, height = %d, ", (*data)->width, (*data)->height);
	free(width);
	free(height);
	skip += strlen(line);
	free(line);
	line = get_next_line(imfd);
	if (!line)
	{
		free(*data);
		return 1;
	}
	skip += strlen(line);
	(*data)->skip = skip;
	free(line);
	close(imfd);
	return 0;
}

//1 lseek(skip)
//2 read(width * height)
//3 ascii luminosity table
//4 pixel value to acscii in a char * with \n based on width

static int pixel_to_ascii(t_data *data, char *path)
{
	int	imfd;
	int	nlcheck;
	unsigned char	*pixel_buff;
	char *ascii_table = " .:-=+*#%@";
	int	luminosity;
	int	r;
	int	g;
	int	b;

	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return 1;
	}
	lseek(imfd, data->skip, SEEK_SET);
	pixel_buff = malloc(sizeof(char) * data->frame_len);
	read(imfd, pixel_buff, data->frame_len);
	int i = 0;

	// while (i < data->frame_len)
	// {
	// 	printf("[%u][%u][%u] ", pixel_buff[i], pixel_buff[i + 1], pixel_buff[i + 2]);
	// 	i += 3;
	// }

	nlcheck = 0;
	while (i < data->frame_len)
	{
		r = pixel_buff[i];
		g = pixel_buff[i + 1];
		b = pixel_buff[i + 2];
		luminosity = (299 * r + 587 * g + 114 * b) / (1000 * 26);
		write(1, &ascii_table[luminosity], 1);

		i += 3;
		nlcheck++;
		if (nlcheck % data->width == 0)
			write(1, "\n", 1);
	}

	free(pixel_buff);
	return 0;
}

int main(int argc, char **argv)
{
	t_data	*data;
	char	*path;

	data = NULL;
	path = argv[1];
	if (argc != 2)
		return 1;
	if (setup(path, &data))
		return 1;
	printf("skip = %d\n", data->skip);
	pixel_to_ascii(data, path);
	free(data);
	return 0;
}

#include "insufflate.h"

char *pixel_to_ascii(t_data *data, char *path, int skip)
{
	int		imfd;
	int		nlcheck;
	unsigned char	*pixel_buff;
	// char		*ascii_table = " .-:~+x%#W";
	char		*ascii_table = " .-:~+x%10";
	int		luminosity;
	int		r;
	int		g;
	int		b;
	char	*frame;

	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return NULL;
	}
	lseek(imfd, skip, SEEK_SET);
	pixel_buff = malloc(sizeof(char) * data->len * 3);
	if (!pixel_buff)
		return NULL;
	read(imfd, pixel_buff, data->len * 3);
	close(imfd);
	int i = 0;

	// while (i < data->len)
	// {
	// 	printf("[%u][%u][%u] ", pixel_buff[i], pixel_buff[i + 1], pixel_buff[i + 2]);
	// 	i += 3;
	// }

	frame = malloc(data->len + data->height + 1);
	if (!frame)
	{
		free(pixel_buff);
		return NULL;
	}
	frame[data->len + data->height] = '\0';
	nlcheck = 0;
	while (i < data->len * 3)
	{
		r = pixel_buff[i];
		g = pixel_buff[i + 1];
		b = pixel_buff[i + 2];
		luminosity = (299 * r + 587 * g + 114 * b) / (1000 * 26);
		frame[nlcheck] = ascii_table[luminosity];

		i += 3;
		nlcheck++;
		if ((nlcheck + 1) % (data->width + 1) == 0)
		{
			frame[nlcheck] = '\n';
			nlcheck++;
		}
	}
	printf("\033[H");
	printf("%s", frame);
	fflush(stdout);
	free(pixel_buff);
	return frame;
}

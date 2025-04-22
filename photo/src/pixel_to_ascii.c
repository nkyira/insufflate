#include "insufflate.h"

int pixel_to_ascii(t_frame_data *data, char *path)
{
	int		imfd;
	int		nlcheck;
	unsigned char	*pixel_buff;
	char		*ascii_table = " .-:~+x=ZW";
	int		luminosity;
	int		r;
	int		g;
	int		b;

	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return 1;
	}
	lseek(imfd, data->skip, SEEK_SET);
	pixel_buff = malloc(sizeof(char) * data->frame_len * 3);
	if (!pixel_buff)
		return 1;
	read(imfd, pixel_buff, data->frame_len * 3);
	close(imfd);
	int i = 0;

	// while (i < data->frame_len)
	// {
	// 	printf("[%u][%u][%u] ", pixel_buff[i], pixel_buff[i + 1], pixel_buff[i + 2]);
	// 	i += 3;
	// }

	data->frame = malloc(data->frame_len + data->height + 1);
	if (!data->frame)
	{
		free(pixel_buff);
		return 1;
	}
	data->frame[data->frame_len + data->height] = '\0';
	nlcheck = 0;
	while (i < data->frame_len * 3)
	{
		r = pixel_buff[i];
		g = pixel_buff[i + 1];
		b = pixel_buff[i + 2];
		luminosity = (299 * r + 587 * g + 114 * b) / (1000 * 26);
		data->frame[nlcheck] = ascii_table[luminosity];

		i += 3;
		nlcheck++;
		if ((nlcheck + 1) % (data->width + 1) == 0)
		{
			data->frame[nlcheck] = '\n';
			nlcheck++;
		}
	}
	printf("\033[2J\033[H");
	printf("%s", data->frame);
	free(pixel_buff);
	return 0;
}

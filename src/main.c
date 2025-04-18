#include "insufflate.h"

int resize_image(int origin_w, int origin_h)
{
//	printf("Resizing image...\n");
	char	cmd[64];
	float	new_w;
	float	new_h;

	if (origin_w / F_WIDTH > origin_h / (F_HEIGHT * 2))
	{
		new_w = F_WIDTH;
		new_h = (float)origin_h / (((float)origin_w / F_WIDTH) * 2);
	}
	else
	{
		new_h = F_HEIGHT;
		new_w = (float)origin_w / ((float)origin_h / (F_HEIGHT * 2));
	}
	snprintf(cmd, sizeof(cmd), "convert temp.ppm -resize %dx%d\\! temp.ppm", (int)new_w, (int)new_h);
	system(cmd);
	return 0;
}

int convert_image(char *path)
{
	char	cmd[64];
	int	origin_w;
	int	origin_h;
	int	imfd;

//	printf("Converting image to .ppm...\n");
	snprintf(cmd, sizeof(cmd), "convert %s temp.ppm", path);
	system(cmd);
	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return -1;
	}
	close(imfd);
	imfd = open("temp.ppm", O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		return -1;
	}
	get_size(imfd, &origin_w, &origin_h);
//	printf("ow = %d, fw = %d, oh = %d, fw = %d\n", origin_w, F_WIDTH, origin_h, F_HEIGHT);
	if (origin_w > F_WIDTH || origin_h > F_HEIGHT)
		resize_image(origin_w, origin_h);
	close(imfd);
	return (0);
}

int main(int argc, char **argv)
{
	t_frame_data	*data;
	char		*path;

	data = NULL;
	path = "temp.ppm";
	if (argc != 2)
		return 1;
	convert_image(argv[1]);
	if (setup(path , &data))
		return 1;
	printf("skip = %d\n", data->skip);
	pixel_to_ascii(data, path);
	make_ascii_image(data);
	free(data->frame);
	free(data);
	return 0;
}

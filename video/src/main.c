#include "insufflate.h"

void player(t_data data)
{
	int	i;

	write(1, "\033[2J", 4);
	for(i = 0; i < data.frame_count; i++)
	{
		write(1, "\033[H", 3);
		write(1, data.all_frames[i], strlen(data.all_frames[i]));
		usleep(41667);
	}
}

int main(int argc, char **argv)
{
	t_data	data;
	char	frame_path[64];
	int		i;

	if (argc != 2)
		return 1;
	setup(&data, argv[1]);
	for(i = 0; i < data.frame_count; i++)
	{
		snprintf(frame_path, sizeof(frame_path), "frames/frame%d.ppm", i + 1);
		data.all_frames[i] = pixel_to_ascii(&data, frame_path, get_skip(frame_path));
	}
	player(data);
	for(i = 0; i < data.frame_count; i++)
		free(data.all_frames[i]);
	free(data.all_frames);
	return 0;
}

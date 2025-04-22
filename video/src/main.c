#include "insufflate.h"

int get_frame(char **frames, char *path)
{
	t_frame_data	*frame_data;
	pid_t		convert_pid;
	int		status;

	frame_data = NULL;
	convert_pid = fork();
	if (convert_pid == 0)
		convert_image(path);
	waitpid(convert_pid, &status, 0);
	if (WEXITSTATUS(status) || setup(".temp.ppm" , &frame_data))
		return 1;
	pixel_to_ascii(frame_data, ".temp.ppm");
	*frames = frame_data->frame;
	free(frame_data);
	return 0;
}

int main(int argc, char **argv)
{
	char	**frames;
	char	path[64];
	int	frame_count;
	int	i;

	if (argc != 2)
		return 1;
	frame_count = atoi(argv[1]);
	frames = malloc(sizeof(char *) * (frame_count + 1));
	frames[frame_count] = NULL;
	i = 0;
	while (i < frame_count)
	{
		snprintf(path, sizeof(path), "frames/frame_%d.jpeg", i + 1);
		if (get_frame(&frames[i], path))
			return 1;
		i++;
		printf("FRAME%d\n", i);
	}
	i = -1;
	while (frames[++i])
	{
		printf("\033[2J\033[H"); // Full clear + reset
		printf("%s", frames[i]);
		printf("FRAME%d\n", i);
		fflush(stdout);
		usleep(41667);
	}
	i = -1;
	while (frames[++i])
		free(frames[i]);
	free(frames);
	return 0;
}

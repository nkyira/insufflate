#include "insufflate.h"

int main(int argc, char **argv)
{
	t_frame_data	*data;
	char		*path;
	pid_t		convert_pid;
	int		status;

	data = NULL;
	path = ".temp.ppm";
	if (argc != 2)
		return 1;
	convert_pid = fork();
	if (convert_pid == 0)
		convert_image(argv[1]);
	waitpid(convert_pid, &status, 0);
	if (status || setup(path , &data))
		return 1;
	printf("skip = %d\n", data->skip);
	pixel_to_ascii(data, path);
	make_ascii_image(data);
	while (1)
	{
		printf("\033[2J\033[H");
		printf("%s",data->frame);
		fflush(stdout);
		usleep(41667);
	}
	free(data->frame);
	free(data);
	return 0;
}

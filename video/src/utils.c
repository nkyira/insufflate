#include "insufflate.h"

void free_perror(char *str, char *s1, char *s2, char *s3)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
	perror(str);
	exit(1);
}

void handle_error(char *str, int fd1, int fd2)
{
	perror(str);
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	exit(1);
}

void init_data(t_data *data, int w, int h, int fc)
{
	data->width = w;
	data->height = h;
	data->len = w * h;
	data->frame_count = fc;
	data->all_frames = malloc(sizeof(char *) * (fc + 1));
	data->all_frames[fc] = NULL;
}

void print_data(t_data *data)
{
	printf("data info :\n");
	printf("width = %d\n", data->width);
	printf("height = %d\n", data->height);
	printf("len = %d\n", data->len);
	printf("frame_count = %d\n", data->frame_count);
}

void ffprobe(char *input)
{
	execlp("ffprobe", "ffprobe", "-v", "error", "-select_streams", "v:0",
		"-show_entries", "stream=duration,width,height", "-of", "csv=p=0",
		input, NULL);
}

void ffmpeg(char *input, int width, int height)
{
	char	fps_scale[64];

	snprintf(fps_scale, sizeof(fps_scale), "fps=24,scale=%d:%d", width, height);
	execlp("ffmpeg", "ffmpeg", "-i", input, "-vf", fps_scale, "frames/frame%d.ppm", NULL);
}

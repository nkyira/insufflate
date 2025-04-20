#include "insufflate.h"

static void resize_image(int origin_w, int origin_h, struct winsize w)
{
	char	dim[64];
	float	new_w;
	float	new_h;

	if ((float)origin_w / w.ws_col > (float)origin_h / ((w.ws_row - 1) * 2))
	{
		new_w = w.ws_col;
		new_h = (float)origin_h / (((float)origin_w / w.ws_col) * 2);
	}
	else
	{
		new_h = (w.ws_row - 1);
		new_w = (float)origin_w / ((float)origin_h / ((w.ws_row - 1) * 2));
	}
	snprintf(dim, sizeof(dim), "%dx%d!", (int)new_w, (int)new_h);
	execlp("convert", "convert", ".temp.ppm", "-resize", dim, ".temp.ppm", NULL);
	exit(1);
	
}

void convert_image(char *path)
{
	pid_t	convert1;
	int	status;
	int	origin_w;
	int	origin_h;
	int	imfd;
	struct	winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
	{
		perror("ioctl");
		exit(1);
	}
	convert1 = fork();
	if (convert1 == 0)
	{
		execlp("convert", "convert", path, ".temp.ppm", NULL);
		exit(1);
	}
	waitpid(convert1, &status, 0);
	if (WEXITSTATUS(status))
		exit(1);
	imfd = open(".temp.ppm", O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		exit(1);
	}
	get_size(imfd, &origin_w, &origin_h);
	if (!origin_w || !origin_h)
		exit(1);
	close(imfd);
	// printf("ow = %d, tw = %d, oh = %d, th = %d, ", origin_w, w.ws_col, origin_h, (w.ws_row - 1));
	fflush(stdout);
	if (origin_w > w.ws_col || origin_h > (w.ws_row - 1))
		resize_image(origin_w, origin_h, w);
	exit(0);
}

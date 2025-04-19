#include "insufflate.h"

static void resize_image(int origin_w, int origin_h, struct winsize w)
{
	char	cmd[64];
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
	snprintf(cmd, sizeof(cmd), "convert .temp.ppm -resize %dx%d\\! .temp.ppm", (int)new_w, (int)new_h);
	exit(system(cmd));
	
}

void convert_image(char *path)
{
	char	cmd[64];
	int	origin_w;
	int	origin_h;
	int	imfd;
	struct		winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
		perror("ioctl");
		exit(1);
	}
	snprintf(cmd, sizeof(cmd), "convert %s .temp.ppm", path);
	if(system(cmd))
		exit(1);
	imfd = open(path, O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		exit(1);
	}
	close(imfd);
	imfd = open(".temp.ppm", O_RDONLY);
	if (imfd < 0)
	{
		perror(path);
		exit(1);
	}
	get_size(imfd, &origin_w, &origin_h);
	close(imfd);
	if (origin_w > w.ws_col || origin_h > (w.ws_row - 1))
		resize_image(origin_w, origin_h, w);
	exit(0);
}

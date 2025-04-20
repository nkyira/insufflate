#include "insufflate.h"

int make_ascii_image(t_frame_data *data)
{
	int outfd;

	outfd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfd < 0)
	{
		perror("open");
		return 1;
	}
	dup2(outfd, STDOUT_FILENO);
	printf("%s", data->frame);
	close(outfd);
	return 0;
}

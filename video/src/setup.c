#include "insufflate.h"

int get_skip(char *path)
{
	int		skip;
	int		ffd;
	char	*line;

	ffd = open(path, O_RDONLY);
	if (ffd < 0)
	{
		perror(path);
		return -1;
	}
	get_next_line(-1);
	line = get_next_line(ffd);
	if (!line)
		return -1;
	skip = strlen(line);
	free(line);
	line = get_next_line(ffd);
	if (!line)
		return -1;
	while (*line == '#')
	{
		skip += strlen(line);
		free(line);
		line = get_next_line(ffd);
		if (!line)
			return -1;
	}
	skip += strlen(line);
	free(line);
	line = get_next_line(ffd);
	close(ffd);
	if (!line)
		return -1;
	skip += strlen(line);
	free(line);
	return skip;
}

static void get_stream_info(char *path, int *pipefd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		handle_error("fork", pipefd[0], pipefd[1]);
	if (pid != 0)
		return;
	close(pipefd[0]);
	if(dup2(pipefd[1], STDOUT_FILENO) < 0)
		handle_error("dup2", pipefd[1], -1);
	close(pipefd[1]);
	ffprobe(path);
	handle_error("execlp", -1, -1);
}

static void process_probe(char *probe_output, float *whd)
{
	char	*width;
	char	*height;
	char	*duration;

	width = strndup(probe_output, strchr(probe_output, ',') - probe_output);
	if(!width)
		free_perror("strdup", probe_output, NULL, NULL);
	height = probe_output + strlen(width) + 1;
	height = strndup(height, strchr(height, ',') - height);
	if(!height)
		free_perror("strdup", probe_output, width, NULL);
	duration = probe_output + strlen(width) + strlen(height) + 2;
	duration = strndup(duration, strchr(duration, '\n') - duration);
	if(!duration)
		free_perror("strdup", probe_output, width, height);

	whd[0] = atof(width);
	whd[1] = atof(height);
	whd[2] = atof(duration);
	free(width);
	free(height);
	free(duration);
	free(probe_output);
}

static void	make_frames(t_data *data, char *path, float *whd)
{
	struct	winsize w;
	float	new_w;
	float	new_h;
	pid_t	pid;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
	{
		perror("ioctl");
		exit(1);
	}
	if (whd[0] / w.ws_col > whd[1] / ((w.ws_row - 1) * 2))
	{
		new_w = w.ws_col;
		new_h = whd[1] / ((whd[0] / w.ws_col) * 2);
	}
	else
	{
		new_h = (w.ws_row - 1);
		new_w = whd[0] / (whd[1] / ((w.ws_row - 1) * 2));
	}
	init_data(data, (int)new_w, (int)new_h, (int)(whd[2] * 24) -1);
	pid = fork();
	if (pid == -1)
		handle_error("fork", -1, -1);
	if (pid != 0)
		return;
	ffmpeg(path, (int)new_w, (int)new_h);
	handle_error("execlp", -1, -1);
}

void setup(t_data *data, char *path)
{
	char	*probe_output;
	int		pipefd[2];
	int		status;
	float	whd[3];

	if (pipe(pipefd) == -1)
		handle_error("pipe", -1, -1);
	get_stream_info(path, pipefd);
	close(pipefd[1]);
	wait(&status);
	if (WEXITSTATUS(status))
		exit(1);
	get_next_line(-1);
	probe_output = get_next_line(pipefd[0]);
	close(pipefd[0]);
	process_probe(probe_output, whd);
	make_frames(data, path, whd);
	wait(&status);
	if (WEXITSTATUS(status))
		exit(1);
	printf("width = %f, height = %f, duration = %f\n", whd[0], whd[1], whd[2]);
}

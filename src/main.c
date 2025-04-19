#include "insufflate.h"

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

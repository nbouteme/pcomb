#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

char *file2buf(const char *file)
{
	char *ret;
	unsigned f;
	int r;

	ret = calloc(1, 512);
	f = 0;
	int fd = open(file, O_RDONLY);
	while((r = read(fd, &ret[f], 512)) > 0)
	{
		f += r;
		ret = realloc(ret, f + 512);
	}
	close(fd);
	return ret;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if(mkfifo("fifo_1455", 0777) == -1)
	{
		if(errno != EEXIST)
		{
			printf("Could'n create fifo");
			return 1;
		}
	}
	int fd_serv = open("fifo_server", O_WRONLY);
	int pid = 1455;
	int length = strlen(argv[1]);
	if(write(fd_serv, &length, sizeof(int)) == -1)
		return 2;
	if(write(fd_serv, argv[1], sizeof(char) * length) == -1)
		return 2;
	if(write(fd_serv, "fifo_1455", sizeof(char)*9) == -1)
		return 2;
	int fd_cl = open("fifo_1455", O_RDONLY);
	char c;
	if(read(fd_cl, &c, sizeof(char)) == -1)
			return 2;
	while(c != '\0')
	{
		printf("%c", c);
		if(read(fd_cl, &c, sizeof(char)) == -1)
			return 2;
	}
	close(fd_serv);
	close(fd_cl);
	return 0;
}

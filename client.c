#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define PAGE_SIZE 4096

int main(int argc, char* argv[])
{
	int rd = -1;
	char fifo_client[10] = "";
	sprintf(fifo_client, "fifo.%d", getpid());
	if(mkfifo(fifo_client, 0777) == -1)
	{
		if(errno != EEXIST)
		{
			printf("Could'n create fifo");
			perror("mkfifo");
			return 1;
		}
	}
	int wr = -1;
	int pid = getpid();
	int fd_serv = open("fifo_server", O_WRONLY);
	int length = strlen(argv[1]);
	if(wr = write(fd_serv, &length, sizeof(int))<0)
	{
		printf("Unable to write data");
	}
	if(write(fd_serv, argv[1], sizeof(char) * length) < 0)
	{
		printf("Unable to write data");
	}
	if(write(fd_serv, &pid, sizeof(int)) < 0)
	{
		printf("Unable to write data");
	}
	int fd_cl = open(fifo_client, O_RDONLY);
	
	char buf[PAGE_SIZE] = "";

	while(1)
	{
		if(rd = read(fd_cl, buf, PAGE_SIZE) < 0)
			perror("Unable to read the data from fifo");
		if(rd == 0)
			break;
		if(write(1, buf, rd) < 0)
		{
			perror("Unable to write file data to stdout");
			break;
		}
	}
	close(fd_serv);
	close(fd_cl);
	return 0;
}

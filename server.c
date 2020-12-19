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
printf("check");
	if(mkfifo("fifo_server", 0777) == -1)
	{
		if(errno != EEXIST)
		{
			perror("Could'n create fifo");
			return 1;
		}
	}
	while(1)
	{
		char buf[PAGE_SIZE] = "";
		int rd = -1;
		int fd_serv = open("fifo_server", O_RDONLY);
		int length = 0;
		if(read(fd_serv, &length, sizeof(int)) < 0)
		{
			printf("Make sure you input correct parametrs");
			continue;
		}
		char* file_name = malloc(sizeof(char)*length);
		if(read(fd_serv, file_name, sizeof(char) * length) < 0)
		{
			printf("Make sure you input correct parameters");
			continue;
		}
		int pid;
		if(read(fd_serv, &pid, sizeof(int)) < 0)
		{
			printf("Make sure you input correct parameters");
			continue;
		}
		char fifo_cl[10] = "";
		sprintf(fifo_cl, "fifo.%d", pid);
		int file_fd = -1;
		if(file_fd = open(file_name, O_RDONLY) < 0)
		{
			perror("Unable to open requested file");
			continue;
		}
		int fd_cl = open(fifo_cl, O_WRONLY);
		while(1)
		{
			if((rd = read(file_fd, buf, PAGE_SIZE)) < 0)
				perror("Unable to read the file");
			if(rd == 0)
				break;
			if(write(fd_cl, buf, rd) < 0)
			{	
				perror("Unable to write file data to fifo");
				break;
			}
		}
		free(file_name);
		close(fd_cl);
		close(fd_serv);
	}
}

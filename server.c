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
	while(1)
	{
		if(mkfifo("fifo_server", 0777) == -1)
		{
			if(errno != EEXIST)
			{
				printf("Could'n create fifo");
				return 1;
			}
		}
		int fd_serv = open("fifo_server", O_RDONLY);
		int length;
		if(read(fd_serv, &length, sizeof(int)) == -1)
			return 2;
		char* file_name = malloc(sizeof(char)*length);
		if(read(fd_serv, file_name, sizeof(char) * length) == -1)
			return 2;
		char* pid = malloc(sizeof(char)*9);;
		if(read(fd_serv, pid, sizeof(char)*9) == -1)
			return 2;
		int fd_cl = open(pid, O_WRONLY);
		FILE* fpt = fopen(file_name, "r");
		char c;
		while(feof(fpt) != 1)
		{
			fscanf(fpt, "%c", &c);
			if(write(fd_cl, &c, sizeof(char)) == -1)
				return 2;
		}	
		c = '\0';
		if(write(fd_cl, &c, sizeof(char)) == -1)
				return 2;
		fclose(fpt);
		free(file_name);
		free(pid);
		close(fd_serv);
	}
}

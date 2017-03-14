#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define SHOWHOST

void show_time(long);
void show_info(struct utmp *);

int main()
{
	struct utmp buff;
	int fd;
	if( (fd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}

	while( read(fd, &buff, sizeof(buff)) == sizeof(buff) )
		show_info(&buff);
	
	close(fd);
	return 0;
}

void show_info(struct utmp *buffp)
{
	if(buffp->ut_type != USER_PROCESS) return;

	printf("%-8.8s", buffp->ut_name);
	printf(" ");
	printf("%-8.8s", buffp->ut_line);
	printf(" ");
	show_time(buffp->ut_time);
	#ifdef SHOWHOST
		if ( buffp->ut_host[0] != '\0' )
			printf(" (%s)", buffp->ut_host);
	#endif
		printf("\n");
}

void show_time(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	printf("%12.12s", cp+4);
}

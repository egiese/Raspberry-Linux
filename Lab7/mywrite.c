#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <utmp.h>
#include <time.h>

#define SHOWHOST

char *show_time(long);
char *show_info(struct utmp *);

int main (int ac, char *av[])
{
	int fd;
	int utmpFD;
	struct utmp utmpBuff;
	char msg[128];
	char buf[BUFSIZ];
	char eof[] = "EOF\n";
	
	if(ac != 3)
	{
		fprintf(stderr,"Usage: mywrite username ttyname\n");
		exit(1);
	}
	
	if( (utmpFD = open(UTMP_FILE, O_RDONLY)) == -1 )
	{
		perror(UTMP_FILE);
		exit(1);
	}
	
	
	if( (fd = open(av[2], O_WRONLY)) == -1 )
	{
		perror(av[2]); 
		exit(1);
	}
	
	while( read(utmpFD, &utmpBuff, sizeof(utmpBuff)) == sizeof(utmpBuff) )
	{	
		if(strcmp(utmpBuff.ut_user,"pi") == 0)	
		{
			char *cp = ctime(&(utmpBuff.ut_time));
			strcpy(msg,"\nMessage from ");
			strcat(msg, utmpBuff.ut_user);
			strcat(msg,"@");
			strcat(msg, utmpBuff.ut_host);
			strcat(msg," on ");
			strcat(msg, utmpBuff.ut_line);
			strcat(msg," on ");
			strcat(msg,cp);
			strcat(msg,"\n");
			break;
		}
	}
	
	write( fd, msg, strlen(msg) );
	
	while( fgets( buf, BUFSIZ, stdin ) != NULL )
		if( write( fd, buf, strlen(buf) ) == -1 )
			break;
	
	write( fd, eof, strlen(eof) );
	
	close(utmpFD);	
	close(fd);				

	return 0;
}

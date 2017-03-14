#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#define BUFFERSIZE 4096

void oops(char *, char *);

int main(int ac, char *av[])
{
	int fd, numRead, curr, i, c;
	char buf[BUFFERSIZE] = {0};
	extern char *optarg; 
	extern int optind;
	int tmpS = 0;
	int tmpB = 0;
	int bFlag = 0;
	int eFlag = 0;
	int nFlag = 0;
	int sFlag = 0;
	int bLineNum = 1;
	int nLineNum = 1;

    /* Flag processing in argument list */
    while( (c = getopt(ac, av, "bens")) != -1)
    {
        switch(c)
        {
            case 'b':
                bFlag = 1;
                break;
            case 'e':
                eFlag = 1;
                break;
            case 'n':
                nFlag = 1;
                break;
            case 's':
                sFlag = 1;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    /* Scan through each argument after flag */
    for(i = optind; i < ac; i++)
    {
        /* Error handling when opening each file */
    	if((fd = open(av[i], O_RDONLY)) == -1)
		    oops("Cannot open ", av[i]);
        
        /* Read from file to buffer, until end is reached */
    	while( (numRead = read(fd, buf, BUFFERSIZE)) > 0)
        {
            /* Once buffer is filled, process each address in buffer */
            for(curr = 0; curr < numRead; curr++)
        	{

			/* nFlag */
			if(nFlag && buf[curr] == '\n')
				printf("%d", nLineNum++);
			fflush(stdout);			


			/* eFlag */
			if(eFlag && buf[curr] == '\r')
				printf(" $");	
			fflush(stdout);	
			
			
			/* bFlag */
			if(bFlag && buf[curr] == '\r')
			{
				if(!isEmptyLine(buf[curr], curr))
					printf("%d ", bLineNum++);
			}
				
			
        		printf("%c", buf[curr]);
        		fflush(stdout);	
        	}
        }
        
        close(fd);
	if(numRead == -1)
		oops("Read error from ", av[i]);
    }
    
    return 0;
}

void oops(char *s1, char *s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}

int isEmptyLine(char *buf, int tmp)
{
	while(buf[tmp] != '\r')
	{
		if(!isspace(buf[tmp]))
			return 0;
		else tmp++;
	}
	return 1;		
}

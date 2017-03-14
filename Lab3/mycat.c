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
	char buf[BUFFERSIZE] = {'\0'};
	extern char *optarg; 
	extern int optind;
	int tmpL = 0;
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
	if(nFlag) printf("%d ", nLineNum++);
	if(bFlag) printf("%d ", bLineNum++);

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
        		/* sFlag squeezes output, eliminating blank lines */
			if(sFlag && buf[curr] == '\n')
			{
				tmpS = curr + 1;

				while(buf[tmpS] != '\n')
				{
					if(isspace(buf[tmpS]))
						tmpS++;
					else
						break;
				}
				if(buf[tmpS] == '\n')
					curr = tmpS-1;				
			}

			/* eFlag puts a '$' at the end of every line */
			if(eFlag && buf[curr] == '\n')
				printf(" $");

			/* Print the current character in the buffer address */
        		printf("%c", buf[curr]);

        		/* nFlag numbers each line, starting from 1 */
			if(nFlag && buf[curr] == '\n')
				printf("%d ", nLineNum++);	
        	
			/* bFlag numbers every non-blank line, starting from 1 */
			if(bFlag && buf[curr] == '\n')
			{
				tmpB = curr + 1;
	
				if(!isEmptyLine(buf, tmpB))
					printf("%d ", bLineNum++);					
			}	
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
	while( (buf[tmp] != '\n') )
	{
		if(!isspace(buf[tmp]))
			return 0;
		tmp++;
	}
	return 1;
}

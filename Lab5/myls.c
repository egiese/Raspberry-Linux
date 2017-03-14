#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/ioctl.h>

void do_ls(char [], int, int, int);
int comparator(const void *, const void *);
int revers_cmp(const void *, const void *);

int main(int ac, char *av[])
{
	int c;
	int aFlag = 0;
	int sFlag = 0;
	int rFlag = 0;
	extern int optind;
	extern char *optarg;

	while( (c = getopt(ac, av, "asr")) != -1)
	{
		switch(c)
		{
		    case 'a':
		        aFlag = 1;
		        break;
		    case 's':
		        sFlag = 1;
		        break;
		    case 'r':
		        rFlag = 1;
		        break;
		    default:
		        exit(EXIT_FAILURE);
		}
	}

	if(ac == 1)
		do_ls(".", aFlag, sFlag, rFlag);
	else
	{
		if(ac == 2 && av[1][0] == '-')
			do_ls(".", aFlag, sFlag, rFlag);
		else
			do_ls(av[optind], aFlag, sFlag, rFlag);
	}

	return 0;
}

void do_ls(char dirname[], int aFlag, int sFlag, int rFlag)
{
	DIR *dirPtr, *dirCtr;
	struct dirent *direntPtr;
	struct dirent *direntCtr;
	struct winsize wbuf;
	int i, j, k;
	int longestDirName = 0;
	int numDirEnt = 0;
	int padding = 0;
	int fileLen = 0;
	int numRows = 0;
	int numCols = 0;
	int wndCols = 0;
	int currCol = 1;

	/* Open directory to count number of entries */
	if( (dirCtr = opendir(dirname) ) == NULL)
		fprintf(stderr, "cannot open %s\n", dirname);
	else
	{
		while((direntCtr = readdir(dirCtr)) != NULL)
			numDirEnt++;
		if( (closedir(dirCtr)) == -1 )
		{
			fprintf(stderr, "close error: %s\n", dirname);
			exit(1);
		}
	}
	
	

	/* Open directory to read entries */
	if( (dirPtr = opendir(dirname) ) == NULL)
		fprintf(stderr, "cannot open %s\n", dirname);
	else
	{
		char *dirNames[numDirEnt];
		numDirEnt = 0;

		
		/* Add names of each entry to an array if not hidden file */
		while( (direntPtr = readdir(dirPtr) ) != NULL)
			if(direntPtr->d_name[0] != '.')
				dirNames[numDirEnt++] = direntPtr->d_name;
			
			
		/* Get number of chars in longest file name */
		for(i = 0; i < numDirEnt; i++)
			if( strlen(dirNames[i]) > longestDirName )
				longestDirName = strlen(dirNames[i]);
		
		
		/* Get number of columns in terminal window */
		if( ioctl(0, TIOCGWINSZ, &wbuf) != -1 )
			wndCols = wbuf.ws_col;
			
		
		/* Calculate number of rows and columns in output window */
		numCols = (int) (wndCols / (longestDirName + 2));
		numRows = (int) (numDirEnt / numCols);
		if( (numDirEnt % numCols) != 0 ) numRows++;
				
			
		/* Forward sort if sFlag is on */
		if( (aFlag && sFlag) || sFlag )
			qsort(dirNames, numDirEnt, sizeof(char*), comparator);


		/* Reverse sort if rFlag is on */
		if( (aFlag && rFlag) || rFlag )
			qsort(dirNames, numDirEnt, sizeof(char*), revers_cmp);


		/* Print file names by columns, padding fnames to longest file length */
		for(i = 0; i < numRows; i++)
		{
			for(j = i; j < numDirEnt; j += numRows)
			{
				fileLen = strlen(dirNames[j]);
				padding = longestDirName - fileLen;
				printf("%s  ", dirNames[j]);
				for(k = 0; k < padding; k++)
					printf(" ");
					
			}
			printf("\n");
		}


		/* Close directory */
		if( (closedir(dirPtr)) == -1 )
		{
			fprintf(stderr, "close error: %s\n", dirname);
			exit(1);
		}
		
		printf("\n\n");
		printf("Longest filename = %d\n", longestDirName);
		printf("wndCols to use = %d\n", wndCols);
		printf("numCols to use = %d\n", numCols);
		printf("numRows to use = %d\n", numRows);
		printf("numDirEnt = %d\n", numDirEnt);
	}
}

/* Compare function for qsort -> forward sorting */
int comparator(const void *pa, const void *pb)
{
	const char **a = (const char **)pa;
	const char **b = (const char **)pb;
	return strcmp(*a, *b);
}

/* Compare function for qsort <- reverse sorting */
int revers_cmp(const void *pa, const void *pb)
{
	const char **a = (const char **)pa;
	const char **b = (const char **)pb;
	return strcmp(*b, *a);
}
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

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
	int i;
	int numDirEnt = 0;

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

		while( (direntPtr = readdir(dirPtr) ) != NULL)
			dirNames[numDirEnt++] = direntPtr->d_name;

		if( (aFlag && sFlag) || sFlag )
			qsort(dirNames, numDirEnt, sizeof(char*), comparator);

		if( (aFlag && rFlag) || rFlag )
			qsort(dirNames, numDirEnt, sizeof(char*), revers_cmp);

		for(i = 0; i < numDirEnt; i++)
		{
			if( (aFlag && sFlag) || (aFlag && rFlag ) || aFlag )
				printf("%s\n", dirNames[i]);

			else if( (sFlag || rFlag) || (!aFlag && !sFlag && !rFlag) )
				if(dirNames[i][0] != '.')
					printf("%s\n", dirNames[i]);
		}

		if( (closedir(dirPtr)) == -1 )
		{
			fprintf(stderr, "close error: %s\n", dirname);
			exit(1);
		}
	}
}

int comparator(const void *pa, const void *pb)
{
	const char **a = (const char **)pa;
	const char **b = (const char **)pb;
	return strcmp(*a, *b);
}
int revers_cmp(const void *pa, const void *pb)
{
	const char **a = (const char **)pa;
	const char **b = (const char **)pb;
	return strcmp(*b, *a);
}

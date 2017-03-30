#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

void do_find(char [], char []);
void printPermissions(char []);
int getNumDirs(char []);
int matchExists(char [], char[]);

int main(int ac, char *av[])
{
	if(ac == 1)
	{
		fprintf(stderr, "Error: %s -- search field required\n", av[0]);
		exit(1);
	}

	else if(ac == 2)
		do_find(av[1], ".");

	else if(ac == 3)
		do_find(av[1], av[2]);

	return 0;
}

void do_find(char index[], char dirname[])
{
	DIR *dirPtr;
	struct dirent *direntPtr;
	char cur[] = ".";
	char par[] = "..";
	int i = 0;
	int j = 0;
	int numDirs = getNumDirs(dirname);
	int matchFound = matchExists(index, dirname);

	if( (dirPtr = opendir(dirname)) == NULL )
	{
		fprintf(stderr, "Cannot open %s\n", dirname);
		exit(1);
	}
	else
	{
		char *dirNames[numDirs];
		char cwd[1024] = {0};
		chdir(dirname);
		getcwd(cwd, sizeof(cwd));

		/* Only print PATH if at least one match is inside */
		if(matchFound)
			printf("%s\n", cwd);

		/* Read through all directory entries and add DIR names to buffer */
		while( (direntPtr = readdir(dirPtr)) != NULL )
		{
			if( direntPtr->d_type == DT_DIR )
				dirNames[i++] = direntPtr->d_name;

			if( strstr(direntPtr->d_name, index) )
				printPermissions(direntPtr->d_name);
		}

		/* Recursively call do_find on DIR names in buffer */
		for(j = 0; j < numDirs; j++)
		{
			if( strcmp(dirNames[j],cur) != 0 && strcmp(dirNames[j],par) != 0)
			{
				do_find(index, dirNames[j]);
				chdir("..");
			}
		}
	}
	if( (closedir(dirPtr)) == -1 )
	{
		fprintf(stderr, "close error: %s\n", dirname);
		exit(1);
	}
}

/* Check the CWD to see if there a match exists inside - for printing PATH */
int matchExists(char index[], char dirname[])
{
	DIR *dirCtr;
	struct dirent *direntCtr;

	if( (dirCtr = opendir(dirname)) == NULL )
	{
		fprintf(stderr, "Cannot open %s\n", dirname);
		exit(1);
	}
	else
	{
		while( (direntCtr = readdir(dirCtr)) != NULL )
			if( strstr(direntCtr->d_name, index) )
				return 1;

		if( (closedir(dirCtr)) == -1 )
		{
			fprintf(stderr, "close error: %s\n", dirname);
			exit(1);
		}
	}
	return 0;
}

/* Function to count the number of directories in a given directory */
int getNumDirs(char dirname[])
{
	DIR *dirCtr;
	struct dirent *direntCtr;
	int numDirs = 0;

	if( (dirCtr = opendir(dirname)) == NULL )
	{
		fprintf(stderr, "Cannot open %s\n", dirname);
		exit(1);
	}
	else
	{
		while( (direntCtr = readdir(dirCtr)) != NULL )
			if(direntCtr->d_type == DT_DIR)
				numDirs++;

		if( (closedir(dirCtr)) == -1 )
		{
			fprintf(stderr, "close error: %s\n", dirname);
			exit(1);
		}
	}
	return numDirs;
}

/* Function to print the permissions in form of (xxxx/----------)
 * for each directory entry that is passed as argument
 */
void printPermissions(char filename[])
{
	struct stat fileS;

	if( stat(filename, &fileS) < 0 )
	{
		printf("File stat error\n");
		exit(1);
	}

	printf("\t%s (", filename);
	if( S_ISDIR(fileS.st_mode) )
		printf("%o", fileS.st_mode&(S_ISDIR(fileS.st_mode)));
	else
		printf("%o", fileS.st_mode&(S_ISUID));
	printf("%o/", fileS.st_mode&(S_IRWXU|S_IRWXG|S_IRWXO));

	if( S_ISDIR(fileS.st_mode) )
		printf("d");
	else
		printf("-");
	if( (fileS.st_mode & S_IRUSR) )
		printf("r");
	else
		printf("-");
	if( (fileS.st_mode & S_IWUSR) )
		printf("w");
	else
		printf("-");
	if( (fileS.st_mode & S_IXUSR) )
		printf("x");
	else
		printf("-");
	if( (fileS.st_mode & S_IRGRP) )
		printf("r");
	else
		printf("-");
	if( (fileS.st_mode & S_IWGRP) )
		printf("w");
	else
		printf("-");
	if( (fileS.st_mode & S_IXGRP) )
		printf("x");
	else
		printf("-");
	if( (fileS.st_mode & S_IROTH) )
		printf("r");
	else
		printf("-");
	if( (fileS.st_mode & S_IWOTH) )
		printf("w");
	else
		printf("-");
	if( (fileS.st_mode & S_IXOTH) )
		printf("x");
	else
		printf("-");
	printf(")\n");
}

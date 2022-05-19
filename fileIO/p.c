#include <stdio.h> 
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
int num=0;
void CHECK_WORD(char* filename,char* Q)
{
	FILE* fp;
	char t;
	int flag=0;


	if((fp= fopen(filename, "rt" )) ==NULL) 
	{
		perror("open");
		exit(1);
	}
	
	while((t=fgetc(fp))!=EOF)
	{
		flag=0;
		if(t==Q[0])
		{
			flag=1;
			for(int i=1; i<strlen(Q); i++)
			{
				t=fgetc(fp);
				if(t!=Q[i])
				{flag=0; break;}
			}
			num+=flag;
		}
	}
	fclose(fp);
	return;
}

void OPEN_DIR(char*file, char* Q)
{
	DIR * directory;
	struct dirent* de;
	struct stat st;
	int count = 0;
	char filename[2000];

	if(!(directory = opendir(file)))
	{
		perror("Failed to open directory");
		exit(1);
	}

	while(0 != (de = readdir(directory)))
	{
		sprintf(filename, "%s/%s", file, de->d_name);
		stat(filename, &st);
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
	       		 continue;
		if(S_ISDIR(st.st_mode))
		{
			OPEN_DIR(filename, Q);
		}
		if(S_ISREG(st.st_mode))
		{
			CHECK_WORD(filename, Q);
		}	

	}
	closedir(directory);
	return;
}

		

int main(int argc, char **argv)
{
	OPEN_DI stat(filename, &st)R(argv[1], argv[2]);
	printf("%d\n", num);
	return 0;
}

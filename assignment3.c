#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>


int readable(char* inputPath) {
	int count = 0;
	char buffer[PATH_MAX];
	struct stat s;
	struct dirent *current;
	DIR *directory;

	//Get current working directory if input is null 
	if(inputPath == NULL) { 
		inputPath = getcwd(buffer, sizeof(buffer));
	}

	//Error checking here
	if (lstat(inputPath, &s) == 0) {

		//It's a regular file 
		if(S_ISREG(s.st_mode)) { 
			return 1; 
		}
		//It's a directory 
		else if(S_ISDIR(s.st_mode)) { 
			directory = opendir(inputPath);
		}
		else { 
			return 0;
		}
	}
	//File doesn't exist or unreachable
	else { 
		return errno*-1; 
	} 

	//Loop to count # of readable files
	while((current = readdir(directory))) {
		char buff[PATH_MAX];

		sprintf(buff, "%s/%s", inputPath, current->d_name);
        printf("\n%s", buff);

        //Directory jumping test
        int forward = strcmp((current->d_name), ".");
        int backward = strcmp((current->d_name), "..");

        if(forward == 0 || (backward == 0)) {
            continue;
        }

        //If directory, do recursive call
        if(current->d_type == DT_DIR && access(buff, R_OK) == 0) {
            count = count + readable(buff);
        }
        else if(current->d_type == DT_REG && access(buff, R_OK) == 0){ 
            count++;
        }

    }
    closedir(directory);
    return count;
}

int main(int argc, char const *argv[]) {
	int blob = readable("../..");
	printf("\n%d\n", blob);
	
	return 0;
}

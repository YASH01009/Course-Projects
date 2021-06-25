#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


#define PATH_LEN 100
#define COMD_LEN 100

// function to print the 
// cwd at the command prompt
void printPrompt() {
	char* cwd = (char*)malloc((size_t)PATH_LEN);
	if(getcwd(cwd, (size_t)PATH_LEN))
		printf("%s $ ", cwd);
	else
		perror("getcwd() error");
}

int main(void) {
	while(1) {
		int childPid;
		char* cmdLine = (char*)malloc(sizeof(COMD_LINE));

		printPrompt();

		// read the cmd
		scanf("%[^\n]", cmdLine);
		getchar();

		
	}
}
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define N 10

int main() {
	int i;
	char *argv_list[] = {"./main_file_01", NULL};

	for(i = 0; i < N; i++) {
		int rc = fork();

		if(rc < 0) {
            exit(1);
		} else if(rc == 0) {
            int exec_ret = execvp(argv_list[0], argv_list);
            printf("Process failed ALERT : %d\n", exec_ret);
        } else {
            int wc = wait(NULL);
        }

        printf("\n");
	}
}
// C Program to design a shell in Linux 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<fcntl.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
  
#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported


// this array tracks the list
// of the last ten commands
char* history[10];
int start_history = -1;
int end_history = -1;
int length_history = 0;

// this function adds the history
// of the cmds into the array
void add_to_history(char* buf) {
	// overlook same cmds
	if(end_history != -1 && 
		strcmp(buf, history[end_history]) == 0)
		return;

	if(start_history == -1 && end_history == -1) {
		start_history = 0;
		end_history = 0;
	} else if(start_history == 0 && end_history == 9) {
		start_history++;
		end_history = 0;
	} else if(start_history == 0) {
		end_history++;
	} else if(start_history == 9) {
		start_history = 0;
		end_history++;
	} else {
		start_history++;
		end_history++;
	}
	history[end_history] = buf;
	if(length_history < 10)
		length_history++;
	return;
}
  
// Function to take input 
int takeInput(char* str) 
{ 
    char* buf; 
  
    buf = readline(" $ "); 
    if (strlen(buf) != 0) { 
        add_to_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
} 
  
// Function to print Current Directory. 
void printDir() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\n%s", cwd); 
} 
  
// Function where the system command is executed 
void execArgs(char** parsed, int background, int cmdLength) 
{ 

	// checking for redirection
	int output_redirect = 0;
	int input_redirect = 0;

	char input_file[100];
	char output_file[100];

	if (cmdLength > 2 && strcmp(parsed[cmdLength - 2], ">") == 0) {
		if (cmdLength > 4 && strcmp(parsed[cmdLength - 4], "<") == 0) {
			output_redirect = 1;
	        input_redirect = 1;

	        strcat(input_file, parsed[cmdLength - 3]);
	        strcat(output_file, parsed[cmdLength - 1]);

	        parsed[cmdLength - 4] = NULL;
	        parsed[cmdLength - 3] = NULL;
	        parsed[cmdLength - 2] = NULL;
	        parsed[cmdLength - 1] = NULL;
		} else {
			output_redirect = 1;
			strcpy(output_file, parsed[cmdLength - 1]);

			parsed[cmdLength - 2] = NULL;
	        parsed[cmdLength - 1] = NULL;
		}
	} 

	if (cmdLength > 2 && strcmp(parsed[cmdLength - 2], "<") == 0) {
		input_redirect = 1;
		strcpy(input_file, parsed[cmdLength - 1]);
		
		parsed[cmdLength - 2] = NULL;
	    parsed[cmdLength - 1] = NULL;
	}

    // Forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) {
    	// handling redirection
    	if (input_redirect == 1) {
    		close(STDIN_FILENO);
    		open(input_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    	}

    	if(output_redirect == 1) {
    		close(STDOUT_FILENO);
    		open(output_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    	}

        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate
        if(background == 0)
        	wait(NULL);  
        return; 
    } 
} 
  
// Function where the piped system commands is executed 
void execArgsPiped(char** parsed, char** parsedpipe) 
{ 
    // 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) { 
        // Child 1 executing.. 
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command 1.."); 
            exit(0); 
        } 
    } else { 
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute command 2.."); 
                exit(0); 
            } 
        } else { 
            // parent executing, waiting for two children
            // as the child processes are tied to pipes, the
            // call doesn't return thought the child exited 
            // normaly, hence using WNOHANG will not block the 
            // execution in the parent process
            // https://stackoverflow.com/questions/37089168/waitpid-not-returning-after-child-has-exited
            waitpid(p1, NULL, WNOHANG);
            waitpid(p2, NULL, WNOHANG);
        } 
    }
    return;
} 
  
// Function to execute builtin commands 
int ownCmdHandler(char** parsed, int length) 
{ 
    int NoOfOwnCmds = 5, i, switchOwnArg = 0, count; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
  
    ListOfOwnCmds[0] = "jobs"; 
    ListOfOwnCmds[1] = "cd"; 
    ListOfOwnCmds[2] = "history"; 
    ListOfOwnCmds[3] = "kill"; 
    ListOfOwnCmds[4] = "exit";
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    case 1: 
        
        return 1;
    case 2: 
        chdir(parsed[1]); 
        return 1; 
    case 3: 
        count = 1;
        for(i = start_history; i < 10 && count <= length_history; i++, count++)
        	printf("%d : %s\n", count, history[i]);
        for(i = 0; count <= length_history && i <= end_history; i++, count++)
        	printf("%d : %s\n", count, history[i]);
        return 1; 
    case 4: 
        
        return 1;
    case 5:
        exit(0);
    default: 
        break; 
    } 
  
    return 0; 
} 
  
// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strpiped[i] = strsep(&str, "|"); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; // returns zero if no pipe is found. 
    else { 
        return 1; 
    } 
} 
  
// function for parsing command words 
int parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    }

    return i;
} 
  
int processString(char* str, char** parsed, char** parsedpipe, int *length) 
{ 
  
    char* strpiped[2]; 
    int piped = 0;
  
    piped = parsePipe(str, strpiped);
  
    if (piped) { 
        parseSpace(strpiped[0], parsed); 
        parseSpace(strpiped[1], parsedpipe); 
  
    } else { 
  
       *length = parseSpace(str, parsed); 
    }

    // returns 3 if cmd is in history
    if(strcmp(parsed[0], "!") == 0)
    	return 3;
  
    if (ownCmdHandler(parsed, *length)) 
        return 0; 
    else
        return 1 + piped; 
}

int background_process(char** parsed, int cmdLength)
{
	if(strcmp(parsed[cmdLength - 1], "&") == 0) {
		parsed[cmdLength - 1] = NULL;
		return 1;
	}
	return 0;
}

// running cmds of form ! num
void run_from_history(char** parsed) {
	int x;
	sscanf(parsed[1], "%d", &x);
	if (x < 0) {
		x += end_history;
		if(x < 0)
			x += 10;
	} else {
		x -= 1;
	    x += start_history;
	    if (x > 9)
		    x -= 9;
	}

	char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    char* parsedArgsPiped[MAXLIST]; 
    int execFlag = 0; 
    int cmdLength = -1;

    strcpy(inputString, history[x]);

    execFlag = processString(inputString, 
    parsedArgs, parsedArgsPiped, &cmdLength); 
    // execflag returns zero if there is no command 
    // or it is a builtin command, 
    // 1 if it is a simple command 
    // 2 if it is including a pipe. 
    // 3 if cmd is in history
 
    // finding if the process is a 
    // background process
    int background = background_process(parsedArgs, cmdLength);

    // execute 
    if (execFlag == 1) 
        execArgs(parsedArgs, background, cmdLength); 
  
    if (execFlag == 2) 
        execArgsPiped(parsedArgs, parsedArgsPiped);

    if (execFlag == 3)
        run_from_history(parsedArgs);

}
  
int main() 
{ 
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    char* parsedArgsPiped[MAXLIST]; 
    int execFlag = 0; 
    int cmdLength = -1;
  
    while (1) { 
        // print shell line 
        printDir(); 
        // take input 
        if (takeInput(inputString)) 
            continue; 
        // process 
        execFlag = processString(inputString, 
        parsedArgs, parsedArgsPiped, &cmdLength); 
        // execflag returns zero if there is no command 
        // or it is a builtin command, 
        // 1 if it is a simple command 
        // 2 if it is including a pipe. 
        // 3 if cmd is in history
 
        // finding if the process is a 
        // background process
        int background = background_process(parsedArgs, cmdLength);

        // execute 
        if (execFlag == 1) 
            execArgs(parsedArgs, background, cmdLength); 
  
        if (execFlag == 2) 
            execArgsPiped(parsedArgs, parsedArgsPiped);

        if (execFlag == 3)
        	run_from_history(parsedArgs);
    } 
    return 0; 
} 
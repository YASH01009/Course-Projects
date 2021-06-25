#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct ListNode {
	int pid;
	struct ListNode* next;
} ListNode;

typedef struct Queue {
	ListNode* front;
	ListNode* back;
	int length;
} Queue;

void add_to_queue(Queue* Q, ListNode* node) {
	if(Q->length == 0) {
		Q->front = node;
		Q->back = node;
		Q->length++;
		return;
	}
	Q->length++;
	Q->back->next = node;
	Q->back = node;
	return;
}

void remove_from_queue(Queue* Q) {
	if(Q->length > 1) {
		ListNode* t = Q->front;
		Q->front = Q->front->next;
		Q->length--;
	}
	if(Q->length == 1) {
		Q->front = NULL;
		Q->back = NULL;
		Q->length = 0;
	}
}

void display_queue(Queue* Q) {
	ListNode* t = Q->front;
	while(t) {
		printf("%d ", t->pid);
		t = t->next;
	}
}

ListNode* create_node(int a) {
	ListNode* t = (ListNode*)malloc(sizeof(ListNode));
	t->pid = a;
	t->next = NULL;
	return t;
}

int create_process() {
	int rc = fork();

	if(rc < 0) {
		exit(0);
	} else if(rc == 0) {
		char *args[]={"./proc",NULL};
		int exec_ret = execvp(args[0], args);
        printf("Process failed ALERT : %d\n", exec_ret);
	} else {
		waitpid(rc, NULL, WNOHANG);
		return rc;
	}
}

int main() {
	// Ready queue
	Queue* ready = (Queue*)malloc(sizeof(Queue));
	ready->front = NULL;
	ready->back = NULL;
	ready->length = 0;

	// Waiting queue
	Queue* wait = (Queue*)malloc(sizeof(Queue));
	wait->front = NULL;
	wait->back = NULL;
	wait->length = 0;

	int i;
	for(i=0; i<10; i++) {
		// create process
		int pid = create_process();

		// add the process to ready queue
		ListNode* t = create_node(pid);
		add_to_queue(ready, t);

		// display queues
		printf("Ready queue : ");
		display_queue(ready);
		printf("\n");

		printf("Wait queue : ");
		display_queue(wait);
		printf("\n");

		printf("\n");
		sleep(2);

		// let the process start running
		int run = ready->front->pid;
		printf("Running process : %d\n", run);
		remove_from_queue(ready);

		printf("Ready queue : ");
		display_queue(ready);
		printf("\n");

		printf("Wait queue : ");
		display_queue(wait);
		printf("\n");

		printf("\n");
		sleep(2);

		// randomly assume a wait operation
		if(i % 3 == 0) {
			add_to_queue(wait, t);

			// display queues
		    printf("Ready queue : ");
		    display_queue(ready);
		    printf("\n");

		    printf("Wait queue : ");
		    display_queue(wait);
		    printf("\n");

		    printf("\n");
		    sleep(2);

		    // push back to ready queue
		    remove_from_queue(wait);
		    add_to_queue(ready, t);

		    // display queues
		    printf("Ready queue : ");
		    display_queue(ready);
		    printf("\n");

		    printf("Wait queue : ");
		    display_queue(wait);
		    printf("\n");

		    printf("\n");
		    sleep(2);

		    // let the process run
		    printf("Running process : %d\n", run);
		    remove_from_queue(ready);

		    printf("Ready queue : ");
		    display_queue(ready);
		    printf("\n");

		    printf("Wait queue : ");
		    display_queue(wait);
		    printf("\n");

		    printf("\n");
		    sleep(2);
		}

		// push back to ready queue at random
		if(i % 4 != 0)
			add_to_queue(ready, t);


		// let the process terminate
		printf("Ready queue : ");
		display_queue(ready);
		printf("\n");

		printf("Wait queue : ");
		display_queue(wait);
		printf("\n");

		printf("\n");
		sleep(2);
	}

	while(ready->length) {
		// let the process run and terminate
		printf("Running process : %d\n", ready->front->pid);
		remove_from_queue(ready);

		printf("Ready queue : ");
		display_queue(ready);
		printf("\n");

		printf("Wait queue : ");
		display_queue(wait);
		printf("\n");

		printf("\n");
		sleep(2);

		// terminate the process

		printf("Ready queue : ");
		display_queue(ready);
		printf("\n");

		printf("Wait queue : ");
		display_queue(wait);
		printf("\n");

		printf("\n");
		sleep(2);
	}

	return 0;
}
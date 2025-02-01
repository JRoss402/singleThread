#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/resource.h>

struct Registers{
	char *RAX,*RBX,*RCX,*RDX,*PC,*R0;
	char *R1,*R2,*R3,*R4,*R5,*R6,*R7,*R8,*R9;
	void *RBP,*RSP;
};

char *states[]={"Running","Ready","Blocked","Terminated"};
char *priority[]={"Low","Medium","High"};
typedef struct{
	char *thread_id;
	char *process_id;
	char  *timestamp;
	struct Registers regs;
	int nice; 
	char *status; 
	char *user_id;
} Thread;

typedef struct{
	char *thread_id;
    char *next;
} Node;

char* get_pid_str(Thread *thread) {
    pid_t pid = getpid();
    char *pid_str = (char*)malloc(20 * sizeof(char));
    snprintf(pid_str, 20, "%d", pid);

    return pid_str;  
}

char * get_uid_str(Thread *thread){
	uid_t euid = geteuid();
	char *euid_str = (char *)malloc(20 * sizeof(char));
	snprintf(euid_str,20,"%d",euid);

	return euid_str;
}

void thread_create(Thread *thread){
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    char time_buff[80];
	memset(time_buff,0,sizeof(time_buff));
    strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", local);
    thread->process_id = get_pid_str(thread); 
    thread->thread_id = thread->process_id;  
	thread->timestamp = time_buff;
    thread->status = "Ready";
	thread->user_id = ("User Id: %d",get_uid_str(thread));
}

char* get_user(int userid){
	if(userid == 0){
	return "Root";
	}else{
	return "Non-Root";
	}
}

int main(int argc,char *argv[]){
	void *rsp,*rbp,*rax,*rbx,*rcx,*rdx;
	void *r8,*r9,*r10,*r11,*r12,*r13,*r14;
	void *r15,*rsi;
	asm volatile (
		 "movq %%rbp, %0\n"
		 "movq %%rsp, %1\n"
		 "movq %%rax, %2\n"
		 "movq %%rbx, %3\n"
		 "movq %%rcx, %4\n"
		 "movq %%rdx, %5\n"
		 "movq %%r8, %6\n"
		 "movq %%r9, %7\n"
		 "movq %%r10, %8\n"
		 "movq %%r11, %9\n"
		 "movq %%r12, %10\n"
		 "movq %%r13, %11\n"
		 "movq %%r14, %12\n"
		 "movq %%r15, %13\n"
		: "=r" (rbp),"=r" (rsp),"=r" (rax),"=r" (rbx),"=r" (rcx),
		  "=r" (rdx),"=r" (r8),"=r" (r9),"=r" (r10),"=r" (r11),"=r" (r12),
		  "=r" (r13),"=r" (r14),"=r" (r15)
		:
		  );
	printf("Creating thread ID:\n");
	Thread *thread = (Thread*)malloc(sizeof(Thread));
	thread_create(thread);
	printf("Thread ID: %s was created\n",thread->thread_id);	
	printf("Process ID: %s => Single Threaded.\n",thread->process_id);
	printf("Time Created: %s\n",thread->timestamp);
	printf("Nice Value: Default(0)\n");
	printf("Status: %s\n",thread->status);
	printf("User ID: %s\n",thread->user_id);
	printf("Register States\n");
	printf("RAX: %p\n",rax);
	printf("RBX: %p\n",rbx);
	printf("RCX: %p\n",rcx);
	printf("RDX: %p\n",rdx);
	printf("RSP: %p\n",rsp);
	printf("RBP: %p\n",rbp);
	printf("R8: %p\n",r8);
	printf("R9: %p\n",r9);
	printf("R10: %p\n",r10);
	printf("R11: %p\n",r11);
	printf("R12: %p\n",r12);
	printf("R13: %p\n",r13);
	printf("R14: %p\n",r14);
	printf("R15: %p\n",r15);
	printf("Completing entry into Thread Control Block\n");
	sleep(2);
	printf("Thread ID: %s is now Running\n",thread->thread_id);
	sleep(2);
	thread->status = "Terminated";
	printf("Thread ID: %s has finished and Terminated\n",thread->thread_id);
	return 0;
}

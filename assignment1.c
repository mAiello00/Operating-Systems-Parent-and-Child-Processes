#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main()
{
	pid_t pid1,pid2,pid3;//create the pid variables
	char c1[20] = " for child_1";
	char c2[20] = " for child_2.1";

	pid1 = fork();//create the first child process (in the parent, pid1 holds the child_1 PID. In the child, pid1 holds 0)
	wait(NULL);//wait for the first child  process to complete before continuing the program
	
	if(pid1 < 0)
	{
		printf("fork unsuccessful\n");
		exit(1);
	}
	
	if(pid1 > 0)
	{
		pid2 = fork();//pid2 now holds the PID for the second child. A new process(child_2 has been created)
		wait(NULL);//parent will wait for child_2 to complete
	
		if(pid2 < 0)
		{
			printf("fork unsuccessful\n");
			exit(1);
		}

		if(pid2 > 0)//now control goes back to the parent
		{
			printf("child_1 and child_2 are complete and process is terminating...\n");
			exit(0);
		}

		if(pid2 == 0)//child_2 will be executed before parent because parent is waiting
		{
			printf("parent(PID %d) created child_2(PID %d) \n",getppid(),getpid());

			pid3 = fork();//child_2 creates child_3 and the pid for child 3 is stored in pid3
			wait(NULL);//child_2 will wait for child_3 to complete

			if(pid3 < 0)
			{
				printf("fork unsuccessful\n");
				exit(1);
			}

			if(pid3 == 0)//if we are in the third child
			{
				printf("child_2(PID %d) created child_2.1(PID %d)\n", getppid(),getpid());
				printf("child_2.1(PID %d) is calling an external program external_program.out and leaving child_2(PID %d)\n", getpid(),getppid());
				
				//converts the pid to a string
				int num2 = getpid();
				char snum2[10];
				sprintf(snum2, "%d", num2);
				
				strcat(snum2,c2);
				
				execl("./external_program.out","./external_program.out",snum2,(char*)NULL);
				//
			}
		}
	}

	if(pid1 == 0)//if this is the first child process
	{
		printf("parent(PID %d) created child_1(PID %d)\n",getppid(),getpid());
		printf("parent(PID %d) is waiting for child_1(PID %d) to complete before creating child_2 \n",getppid(), getpid());

		printf("child_1(PID %d) is calling an external program external_program.out and leaving parent\n", getpid());
		
		//converts the pid to a string
		int num = getpid();
		char snum[10];
		sprintf(snum, "%d", num);
		
		strcat(snum,c1);
		
		execl("./external_program.out","./external_program.out",snum, (char*)NULL);
	}


	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

// #include "ComputeTask.h"
// #include "IOTask.h"

typedef enum OP{

	COMPUTE_TASK=1,
	IO_TASK
}OP_t;

typedef struct task{
	struct task *next;
	OP_t taskType;
}task_t;

int main(int argc, char *argv[])
{
	double computeTaskTimeElapse=0.0;
	double IOTaskTimeElapse=0.0;
	double totalTimeElapse=0.0;

	struct timeval computeTaskStartTime, computeTaskEndTime,IOTaskStartTime, IOTaskEndTime;
	
	pid_t fpid;
	task_t computeTask, ioTask;
	task_t* curTask = &ioTask;

	computeTask.taskType = COMPUTE_TASK;
	ioTask.next=&computeTask;
	ioTask.taskType=IO_TASK;
	computeTask.next=NULL;

	int parentProcess = 1;
	int childProcessNum = 0;

	while(NULL!=curTask)
	{
		if(curTask->taskType==IO_TASK)
			gettimeofday(&IOTaskStartTime,NULL);
		else
			gettimeofday(&computeTaskStartTime,NULL);
		fpid=fork();

        // 一旦fork，后面的代码就要判定父子进程了。
		if(0==fpid)     // fork返回0，这里是子进程
		{
			parentProcess=0;
			break;
		}
		else if(-1==fpid)   // fork返回负值，表示子进程创建失败
		{
			printf("Generate child Process error!\n");
			exit(0);
		}

        // 对于父进程，fork返回子进程ID。前面子进程单独处理了，这里父进程只需要继续循环即可
		wait(NULL);     // 其作用是是当前最近的一个子进程结束后，父进程才继续执行。一般用于父进程中
		
		if(COMPUTE_TASK==curTask->taskType)
			gettimeofday(&computeTaskEndTime,NULL);
		else
			gettimeofday(&IOTaskEndTime,NULL);
		printf("Generate child process with pid:%d\n",fpid);

		++childProcessNum;
		curTask=curTask->next;

	}

    // 子进程break出来，会直接到这里；父进程也会到这里。通过parentProcess来判定父子进程，分别执行不同代码。
	if(parentProcess==0)    // parentProcess==0，说明这个是子进程
	{
		if(curTask->taskType==IO_TASK)
		{
			executeIOTask();
			printf("This is a IO task, pid:%d parent pid:%d\n",getpid(),getppid());//Print process ID and parent process ID
		}
		if(curTask->taskType==COMPUTE_TASK)
		{
			executeComputeTask();
			printf("This is a compute task, pid:%d parent pid:%d\n",getpid(),getppid());//Print process ID and parent process ID
		}
	}
	else    // parentProcess==1，说明这个是父进程
	{
		//Parent Process, we calculate the time for executing computing task and the time fo executing IO task
		computeTaskTimeElapse = (double)(computeTaskEndTime.tv_sec - computeTaskStartTime.tv_sec)*1000.0+(double)(computeTaskEndTime.tv_usec - computeTaskStartTime.tv_usec)/1000.0;
		IOTaskTimeElapse = (double)(IOTaskEndTime.tv_sec - IOTaskStartTime.tv_sec)*1000.0+(double)(IOTaskEndTime.tv_usec - IOTaskStartTime.tv_usec)/1000.0;
		totalTimeElapse = (double)(computeTaskEndTime.tv_sec-IOTaskStartTime.tv_sec)*1000.0+(double)(computeTaskEndTime.tv_usec-IOTaskStartTime.tv_usec)/1000.0;
		printf("Compute Task Time Consume:%fms, IO Task Time Consume: %fms, Total Time Consume:%fms \n", computeTaskTimeElapse,IOTaskTimeElapse,totalTimeElapse);
	}


}


#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "string.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100


struct itimerval New, Old;
struct sigaction Sigaction;


//if argument is 1 start the alarm else stop
int Alarm(int alarmType){
	int usec = 0;
	if(alarmType == 1){
		usec = 1000000/HZ;
	}
	New.it_interval.tv_usec = usec;
	New.it_interval.tv_sec = 0;
	New.it_value.tv_usec = usec;
	New.it_value.tv_sec = 0;
	if (setitimer (ITIMER_VIRTUAL, &New, &Old) < 0)
		return 0;
	return 1;
}

void AlarmHandler(){
	
	uthread_block();
	return;
}

void preempt_disable(void)
{
	if(sigprocmask(SIG_UNBLOCK, &Sigaction.sa_mask, NULL) == -1){
		return;
	}
}

void preempt_enable(void)
{
	if(sigprocmask(SIG_UNBLOCK, &Sigaction.sa_mask, NULL) == -1){
		return;
	}

}

void preempt_start(bool preempt)
{
	
	if(preempt){
		//preemption = (PreemptiveController*)malloc(sizeof(PreemptiveController));
		sigemptyset(&Sigaction.sa_mask);
		Sigaction.sa_flags = 0;
		Sigaction.sa_handler = AlarmHandler;
		sigaction(SIGVTALRM, &Sigaction, NULL);
		sigaddset(&Sigaction.sa_mask, SIGVTALRM);
		sigprocmask(SIG_UNBLOCK, &Sigaction.sa_mask, NULL);
		
		Alarm(1);

	}
}

void preempt_stop(void)
{
	Alarm(0);
	Sigaction.sa_handler = SIG_DFL;
	if(sigaction(SIGVTALRM, &Sigaction, NULL)){
		return;
	}
}


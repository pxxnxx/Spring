#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>

#define SEED 10

// process states
#define S_IDLE 0			
#define S_READY 1
#define S_BLOCKED 2
#define S_RUNNING 3
#define S_TERMINATE 4

int NPROC, NIOREQ, QUANTUM;

struct ioDoneEvent {
	int procid;
	int doneTime;
	int len;
	struct ioDoneEvent *prev;
	struct ioDoneEvent *next;
} ioDoneEventQueue, *ioDoneEvent;

struct process {
	int id;
	int len;		// for queue
	int targetServiceTime;
	int serviceTime;
	int startTime;
	int endTime;
	char state;
	int priority;
	int saveReg0, saveReg1;
	struct process *prev;
	struct process *next;
} *procTable;

struct process idleProc;
struct process readyQueue;
struct process blockedQueue;
struct process *runningProc;

int cpuReg0, cpuReg1;
int currentTime = 0;
int *procIntArrTime, *procServTime, *ioReqIntArrTime, *ioServTime;

void compute() {
	// DO NOT CHANGE THIS FUNCTION
	cpuReg0 = cpuReg0 + runningProc->id;
	cpuReg1 = cpuReg1 + runningProc->id;
	//printf("In computer proc %d cpuReg0 %d\n",runningProc->id,cpuReg0);
}

void initProcTable() {
	int i;
	for(i=0; i < NPROC; i++) {
		procTable[i].id = i;
		procTable[i].len = 0;
		procTable[i].targetServiceTime = procServTime[i];
		procTable[i].serviceTime = 0;
		procTable[i].startTime = 0;
		procTable[i].endTime = 0;
		procTable[i].state = S_IDLE;
		procTable[i].priority = 0;
		procTable[i].saveReg0 = 0;
		procTable[i].saveReg1 = 0;
		procTable[i].prev = NULL;
		procTable[i].next = NULL;
	}
} // len, serviceTime , saveReg

void procExecSim(struct process *(*scheduler)()) {
	int pid, qTime=0, cpuUseTime = 0, nproc=0, termProc = 0, nioreq=0;
	char schedule = 0, nextState = S_IDLE;
	int nextForkTime, nextIOReqTime;
    runningProc = &idleProc;
	nextForkTime = procIntArrTime[nproc];
	nextIOReqTime = ioReqIntArrTime[nioreq];
    idleProc.targetServiceTime = -1;
	blockedQueue.id = -2;
	ioDoneEventQueue.procid = -1;
	readyQueue.id = INT_MAX; 
    readyQueue.targetServiceTime = 1000;
    int i;

	while(1) {
        int ioDone = 0;
        int checkQuant = 0;
        if (termProc == NPROC) {
            break;
        }

        if (readyQueue.next != &readyQueue && runningProc == &idleProc) {
            scheduler();
            runningProc = readyQueue.next;
            runningProc->next->prev = &readyQueue;
            readyQueue.next = runningProc->next;
            runningProc->state = S_RUNNING; 
            cpuReg0 = runningProc->saveReg0;
            cpuReg1 = runningProc->saveReg1;
            if (runningProc->startTime == 0)
                runningProc->startTime  = currentTime;
        }
        currentTime++;
		if (runningProc != &idleProc) {cpuUseTime++; qTime++; runningProc->serviceTime++;} // **not Idle, cputime increase

		compute(); 
		if (currentTime == nextForkTime) { /* CASE 2 : a new process created */
            readyQueue.prev->next = &procTable[nproc];
            procTable[nproc].prev = readyQueue.prev;
            procTable[nproc].next = &readyQueue;
            readyQueue.prev = &procTable[nproc];
            procTable[nproc].startTime = currentTime;
            nproc++;
            nextForkTime = currentTime + procIntArrTime[nproc];
            if (runningProc != &idleProc) {
                if (runningProc->serviceTime == runningProc->targetServiceTime) { 
                    nextState = S_TERMINATE;
                    runningProc->endTime = currentTime;              
                }
                else {
                    nextState = S_READY;
                }
            }
		}

     

		if (qTime == QUANTUM || runningProc->serviceTime == runningProc->targetServiceTime) { /* CASE 1 : The quantum expires */
            checkQuant = 1;
            runningProc->priority--;
            qTime = 0;
            if (runningProc->serviceTime == runningProc->targetServiceTime) { // 퀀텀과 동시에 프로세스 종료
                nextState = S_TERMINATE;
                runningProc->endTime = currentTime;            
            }
            else {
                nextState = S_READY;
            }
		}

		while (ioDoneEventQueue.next->doneTime == currentTime) { /* CASE 3 : IO Done Event */
            ioDone = 1;
            struct process *p = &blockedQueue;
            if (ioDoneEventQueue.next->procid != -1) {
                while (ioDoneEventQueue.next->procid != p->id) {
                    p = p->next;
                }
                p->next->prev = p->prev;
                p->prev->next = p->next;

                p->next = &readyQueue;
                p->prev = readyQueue.prev;
                readyQueue.prev->next = p;
                readyQueue.prev = p;
            }
            else {
                termProc++;
            }

			qTime = 0; 
			ioDoneEventQueue.next->doneTime = currentTime;
			ioDoneEventQueue.next->next->prev = &ioDoneEventQueue;
			ioDoneEventQueue.next = ioDoneEventQueue.next->next;
		}
        if (ioDone == 1 && runningProc != &idleProc) {
            if (runningProc->serviceTime == runningProc->targetServiceTime) {
                nextState = S_TERMINATE;
                runningProc->endTime = currentTime;              
            }
            else {
                nextState = S_READY;
            }
        }

		if (cpuUseTime == nextIOReqTime) { /* CASE 5: reqest IO operations (only when the process does not terminate) */
			if (nioreq < NIOREQ) {
				if (runningProc == &idleProc) {
                    nextIOReqTime++; 
                }
				if (runningProc != &idleProc) {
                    if (checkQuant == 0)
                        runningProc->priority++;
                    runningProc->saveReg0 = cpuReg0;
                    runningProc->saveReg1 = cpuReg1;
					qTime = 0; 
					cpuUseTime = 0; 

					ioDoneEvent[nioreq].doneTime = currentTime + ioServTime[nioreq];

					struct ioDoneEvent *p = &ioDoneEventQueue;
					while (p->next->doneTime <= ioDoneEvent[nioreq].doneTime) {
						if (p->next == &ioDoneEventQueue)
							break;
						p = p->next;
					}
					ioDoneEvent[nioreq].next = p->next;
					ioDoneEvent[nioreq].prev = p->prev;
					p->next->prev = &ioDoneEvent[nioreq];
					p->next = &ioDoneEvent[nioreq];

                    if (nextState == S_READY || nextState == S_IDLE) {
                        runningProc->state = S_BLOCKED;
                    
                        ioDoneEvent[nioreq].procid = runningProc->id; 
                        runningProc->next = &blockedQueue;
                        runningProc->prev = blockedQueue.prev;
                        blockedQueue.prev->next = runningProc;
                        blockedQueue.prev = runningProc;
                        nextState = S_IDLE;
                    }
                    else if (nextState == S_TERMINATE) {
                        ioDoneEvent[nioreq].procid = -1;
                        nextState = S_IDLE;
                    }
                    runningProc = &idleProc;
                    nioreq++;
                    nextIOReqTime = ioReqIntArrTime[nioreq]; 
				}
			}
        }

        if (nextState == S_READY  && runningProc != &idleProc) {
            qTime = 0;
            runningProc->saveReg0 = cpuReg0;
            runningProc->saveReg1 = cpuReg1;
            nextState = S_IDLE;
            runningProc->state = S_READY;
            runningProc->next = &readyQueue;
            runningProc->prev = readyQueue.prev;
            readyQueue.prev->next = runningProc;
            readyQueue.prev = runningProc;
            runningProc = &idleProc;
        }
        else if (nextState == S_TERMINATE && runningProc != &idleProc) {
            qTime = 0;
            runningProc->saveReg0 = cpuReg0;
            runningProc->saveReg1 = cpuReg1;
            nextState = S_IDLE;
            termProc++;
            runningProc = &idleProc;
        }
		if (runningProc->serviceTime == runningProc->targetServiceTime) { /* CASE 4 : the process job done and terminates */
            qTime = 0;
            runningProc->saveReg0 = cpuReg0;
            runningProc->saveReg1 = cpuReg1;
            runningProc->state = S_TERMINATE;
            runningProc->endTime = currentTime;
            termProc++;
            runningProc = &idleProc;

        }

	} // while loop
}

//RR,SJF(Modified),SRTN,Guaranteed Scheduling(modified),Simple Feed Back Scheduling
struct process* RRschedule() {
}
struct process* SJFschedule() {
    struct process *s = readyQueue.next;
    struct process *key = &readyQueue;
    float min = INT_MAX;
    while (s!= &readyQueue) {
        if (min > s->targetServiceTime) {
            min = s->targetServiceTime;
            key = s;
        }
        s = s->next;
    }
    if (key != &readyQueue && key != readyQueue.next) {
		key->prev->next = key->next;
		key->next->prev = key->prev;
		key->next = readyQueue.next;
		readyQueue.next->prev = key;
		key->prev = &readyQueue;
		readyQueue.next = key; 
	}

}
struct process* SRTNschedule() {
    struct process *s = readyQueue.next;
    struct process *key = &readyQueue;
    float min = INT_MAX;
    while (s!= &readyQueue) {
        if (min > (s->targetServiceTime - s->serviceTime)) {
            min = s->targetServiceTime - s->serviceTime;
            key = s;
        }
        s = s->next;
    }
    if (key != &readyQueue && key != readyQueue.next) {
		key->prev->next = key->next;
		key->next->prev = key->prev;
		key->next = readyQueue.next;
		readyQueue.next->prev = key;
		key->prev = &readyQueue;
		readyQueue.next = key; 
	}
    
}
struct process* GSschedule() {
    struct process *s = readyQueue.next;
    struct process *key = &readyQueue;
    float min = INT_MAX;
    while (s!= &readyQueue) {
        if (min > (float)s->serviceTime/(float)s->targetServiceTime) {
            min = (float)s->serviceTime/(float)s->targetServiceTime;
            key = s;
        }
        s = s->next;
    }
    if (key != &readyQueue && key != readyQueue.next) {
		key->prev->next = key->next;
		key->next->prev = key->prev;
		key->next = readyQueue.next;
		readyQueue.next->prev = key;
		key->prev = &readyQueue;
		readyQueue.next = key; 
	}
}
struct process* SFSschedule() {
    struct process *s = readyQueue.next;
    struct process *key = &readyQueue;
    int max = INT_MIN;
    while (s!= &readyQueue) {
        if (max < s->priority) {
            max = s->priority;
            key = s;
        }
        s = s->next;
    }
    if (key != &readyQueue && key != readyQueue.next) {
		key->prev->next = key->next;
		key->next->prev = key->prev;
		key->next = readyQueue.next;
		readyQueue.next->prev = key;
		key->prev = &readyQueue;
		readyQueue.next = key; 
	}

}

void printResult() {
	// DO NOT CHANGE THIS FUNCTION
	int i;
	long totalProcIntArrTime=0,totalProcServTime=0,totalIOReqIntArrTime=0,totalIOServTime=0;
	long totalWallTime=0, totalRegValue=0;
    // totalWallTime += 3;
	for(i=0; i < NPROC; i++) {
		totalWallTime += procTable[i].endTime - procTable[i].startTime;
		/*
		printf("proc %d serviceTime %d targetServiceTime %d saveReg0 %d\n",
			i,procTable[i].serviceTime,procTable[i].targetServiceTime, procTable[i].saveReg0);
		*/
		totalRegValue += procTable[i].saveReg0+procTable[i].saveReg1;
		/* printf("reg0 %d reg1 %d totalRegValue %d\n",procTable[i].saveReg0,procTable[i].saveReg1,totalRegValue);*/
	}
	for(i = 0; i < NPROC; i++ ) { 
		totalProcIntArrTime += procIntArrTime[i];
		totalProcServTime += procServTime[i];
	}
	for(i = 0; i < NIOREQ; i++ ) { 
		totalIOReqIntArrTime += ioReqIntArrTime[i];
		totalIOServTime += ioServTime[i];
	}
	
	printf("Avg Proc Inter Arrival Time : %g \tAverage Proc Service Time : %g\n", (float) totalProcIntArrTime/NPROC, (float) totalProcServTime/NPROC);
	printf("Avg IOReq Inter Arrival Time : %g \tAverage IOReq Service Time : %g\n", (float) totalIOReqIntArrTime/NIOREQ, (float) totalIOServTime/NIOREQ);
	printf("%d Process processed with %d IO requests\n", NPROC,NIOREQ);
	printf("Average Wall Clock Service Time : %g \tAverage Two Register Sum Value %g\n", (float) totalWallTime/NPROC, (float) totalRegValue/NPROC);
	
}

int main(int argc, char *argv[]) {
	// DO NOT CHANGE THIS FUNCTION
	int i;
	int totalProcServTime = 0, ioReqAvgIntArrTime;
	int SCHEDULING_METHOD, MIN_INT_ARRTIME, MAX_INT_ARRTIME, MIN_SERVTIME, MAX_SERVTIME, MIN_IO_SERVTIME, MAX_IO_SERVTIME, MIN_IOREQ_INT_ARRTIME;
	
	if (argc < 12) {
		printf("%s: SCHEDULING_METHOD NPROC NIOREQ QUANTUM MIN_INT_ARRTIME MAX_INT_ARRTIME MIN_SERVTIME MAX_SERVTIME MIN_IO_SERVTIME MAX_IO_SERVTIME MIN_IOREQ_INT_ARRTIME\n",argv[0]);
		exit(1);
	}
	
	SCHEDULING_METHOD = atoi(argv[1]);
	NPROC = atoi(argv[2]);
	NIOREQ = atoi(argv[3]);
	QUANTUM = atoi(argv[4]);
	MIN_INT_ARRTIME = atoi(argv[5]);
	MAX_INT_ARRTIME = atoi(argv[6]);
	MIN_SERVTIME = atoi(argv[7]);
	MAX_SERVTIME = atoi(argv[8]);
	MIN_IO_SERVTIME = atoi(argv[9]);
	MAX_IO_SERVTIME = atoi(argv[10]);
	MIN_IOREQ_INT_ARRTIME = atoi(argv[11]);
	
	printf("SIMULATION PARAMETERS : SCHEDULING_METHOD %d NPROC %d NIOREQ %d QUANTUM %d \n", SCHEDULING_METHOD, NPROC, NIOREQ, QUANTUM);
	printf("MIN_INT_ARRTIME %d MAX_INT_ARRTIME %d MIN_SERVTIME %d MAX_SERVTIME %d\n", MIN_INT_ARRTIME, MAX_INT_ARRTIME, MIN_SERVTIME, MAX_SERVTIME);
	printf("MIN_IO_SERVTIME %d MAX_IO_SERVTIME %d MIN_IOREQ_INT_ARRTIME %d\n", MIN_IO_SERVTIME, MAX_IO_SERVTIME, MIN_IOREQ_INT_ARRTIME);
	
	srandom(SEED);
	
	// allocate array structures
	procTable = (struct process *) malloc(sizeof(struct process) * NPROC);
	ioDoneEvent = (struct ioDoneEvent *) malloc(sizeof(struct ioDoneEvent) * NIOREQ);
	procIntArrTime = (int *) malloc(sizeof(int) * NPROC);
	procServTime = (int *) malloc(sizeof(int) * NPROC);
	ioReqIntArrTime = (int *) malloc(sizeof(int) * NIOREQ);
	ioServTime = (int *) malloc(sizeof(int) * NIOREQ);

	// initialize queues
	readyQueue.next = readyQueue.prev = &readyQueue;
	
	blockedQueue.next = blockedQueue.prev = &blockedQueue;
	ioDoneEventQueue.next = ioDoneEventQueue.prev = &ioDoneEventQueue;
	ioDoneEventQueue.doneTime = INT_MAX;
	ioDoneEventQueue.procid = -1;
	ioDoneEventQueue.len  = readyQueue.len = blockedQueue.len = 0;
	
	// generate process interarrival times
	for(i = 0; i < NPROC; i++ ) { 
		procIntArrTime[i] = random()%(MAX_INT_ARRTIME - MIN_INT_ARRTIME+1) + MIN_INT_ARRTIME;
	}
	
	// assign service time for each process
	for(i=0; i < NPROC; i++) {
		procServTime[i] = random()% (MAX_SERVTIME - MIN_SERVTIME + 1) + MIN_SERVTIME;
		totalProcServTime += procServTime[i];	
	}
	
	ioReqAvgIntArrTime = totalProcServTime/(NIOREQ+1);
	assert(ioReqAvgIntArrTime > MIN_IOREQ_INT_ARRTIME);
	
	// generate io request interarrival time
	for(i = 0; i < NIOREQ; i++ ) { 
		ioReqIntArrTime[i] = random()%((ioReqAvgIntArrTime - MIN_IOREQ_INT_ARRTIME)*2+1) + MIN_IOREQ_INT_ARRTIME;
	}
	
	// generate io request service time
	for(i = 0; i < NIOREQ; i++ ) { 
		ioServTime[i] = random()%(MAX_IO_SERVTIME - MIN_IO_SERVTIME+1) + MIN_IO_SERVTIME;
	}
	
#ifdef DEBUG
	// printing process interarrival time and service time
	printf("Process Interarrival Time :\n");
	for(i = 0; i < NPROC; i++ ) { 
		printf("%d ",procIntArrTime[i]);
	}
	printf("\n");
	printf("Process Target Service Time :\n");
	for(i = 0; i < NPROC; i++ ) { 
		printf("%d ",procTable[i].targetServiceTime);
	}
	printf("\n");
#endif
	
	// printing io request interarrival time and io request service time
	printf("IO Req Average InterArrival Time %d\n", ioReqAvgIntArrTime);
	printf("IO Req InterArrival Time range : %d ~ %d\n",MIN_IOREQ_INT_ARRTIME,
			(ioReqAvgIntArrTime - MIN_IOREQ_INT_ARRTIME)*2+ MIN_IOREQ_INT_ARRTIME);
			
#ifdef DEBUG		
	printf("IO Req Interarrival Time :\n");
	for(i = 0; i < NIOREQ; i++ ) { 
		printf("%d ",ioReqIntArrTime[i]);
	}
	printf("\n");
	printf("IO Req Service Time :\n");
	for(i = 0; i < NIOREQ; i++ ) { 
		printf("%d ",ioServTime[i]);
	}
	printf("\n");
#endif
	
	struct process* (*schFunc)();
	switch(SCHEDULING_METHOD) {
		case 1 : schFunc = RRschedule; break;
		case 2 : schFunc = SJFschedule; break;
		case 3 : schFunc = SRTNschedule; break;
		case 4 : schFunc = GSschedule; break;
		case 5 : schFunc = SFSschedule; break;
		default : printf("ERROR : Unknown Scheduling Method\n"); exit(1);
	}
	initProcTable();
	procExecSim(schFunc);
	printResult();
	
}

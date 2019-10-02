// Pre-compiler directive
#ifndef SIMULATION_H
#define SIMULATION_H

// header files
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "StringUtils.h"
#include "ConfigAccess.h"
#include "MetaDataAccess.h"
#include "simtimer.h"

// global constants

typedef enum { PTHREAD_ERROR = 3,
			   PROCESS_COMPLETE} SimCodeMessages;

// Struct to hold config and meta data
typedef struct Data
{
	struct ConfigDataType *configDataPtr;
	struct OpCodeType *mdDataPtr;
	struct ProcessData *processDataStart;
} Data;

// Timer structure
typedef struct Timer
{
	char timerString[10];
	double timerAccess;
} Timer;

// List of States
typedef enum { NEW,
			   READY,
			   RUNNING,
			   WAITING,
			   EXIT} STATE;
			   
// PCB structure
typedef struct PCB
{
	char opLtr;
	char opName[100];
	int processTime;
	
	struct PCB *next;
} PCB;

// Process structure
typedef struct Process
{
	int processNumber;
	STATE state;
	struct PCB *currentProcess;
} Process;

// Process data structure
typedef struct ProcessData
{
	char data[80];
	
	struct ProcessData *next;
} ProcessData;

// function prototypes
void runSimulation(ConfigDataType *configDataPtr, OpCodeType *mdDataPtr);
PCB *createPCB(Data *data, int *totalTime, PCB *pcb, Process *localProcess);
SimCodeMessages runProcess(Process *process, ProcessData *processData, Timer *timer, Boolean MONITOR_FLAG);
void *ioProcess(void *arg);
void logToFile(FILE *filePtr, ProcessData *processDataPtr);
PCB *addPCBnode(PCB *localPtr, PCB *newNode);
ProcessData *addProcessDataNode(ProcessData *localPtr, ProcessData *newNode);
ProcessData *clearProcessDataList(ProcessData *localPtr);
PCB *clearPCBlist(PCB *localPtr);

#endif //SIMULATION_H
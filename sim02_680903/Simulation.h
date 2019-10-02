// Pre-compiler directive
#ifndef SIMULATION_H
#define SIMULATION_H

// header files
#include <stdio.h>
#include "StringUtils.h"
#include "ConfigAccess.h"
#include "MetaDataAccess.h"
#include "simtimer.h"

// global constants

// Struct to hold config and meta data
typedef struct Data
{
	struct ConfigDataType *configDataPtr;
	struct OpCodeType *mdDataPtr;
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
	double time;
	char process[10]; // OS or Process
	char opLtr;
	char opName[100]; // length of op name - 99 characters
	
	struct ProcessData *next;
} ProcessData;

// function prototypes
void runSimulation(ConfigDataType *configDataPtr, OpCodeType *mdDataPtr);
void createPCB(Data *data, int *totalTime, PCB *pcb, Process *localProcess);
void runProcess(Process *process, ProcessData *processData, Timer *timer);
void logToFile(Data *data, int timeLeft, PCB *pcb, Process *localProcess, ProcessData *processDataPtr, Timer *timer);
PCB *addPCBnode(PCB *localPtr, PCB *newNode);
ProcessData *addProcessNode(ProcessData *localPtr, ProcessData *newNode);
ProcessData *clearProcessDataList(ProcessData *localPtr);
PCB *clearPCBlist(PCB *localPtr);

#endif //SIMULATION_H
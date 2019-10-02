// header files
#include "Simulation.h"

/*
Function Name: startSimulation
Algorithm: Starts the simulation by
Precondition: 
Postcondition: 
Exceptions: none
Notes: none
*/
void runSimulation(ConfigDataType *configData, OpCodeType *mdData)
{
	// initialize function/variables
	char logToCode[STD_STR_LEN];
	char fileName[STD_STR_LEN];
	FILE *filePtr;
	
	PCB *pcb = NULL;
	Process *localProcess;	
	ProcessData *processDataPtr = NULL;
	Timer *timer;
	Data *data;
	
	int timeLeft = 0;
	
	// allocate memory for the temporary data structures
		// function: malloc	
	timer = (Timer *) malloc(sizeof(Timer));
	localProcess = (Process *) malloc(sizeof(Process));
	data = (Data *) malloc(sizeof(Data));
	
	// Set the currentProcess to be NULL
	localProcess->currentProcess = NULL;
	
	// store the combined data in a struct for easier function calling
	data->configDataPtr = configData;
	data->mdDataPtr = mdData;
	
	// Get the file name from config data and open file for writing
		// function:copyString, fopen
	copyString(fileName, configData->logToFileName);
	filePtr = fopen(fileName, WRITE_ONLY_FLAG);
	
	// Get Log To Code
		// function: configCodeToString
	configCodeToString(configData->logToCode, logToCode);
	
	// set logToCode to lowercase for testing
		// function: setStrToLowerCase
	setStrToLowerCase(logToCode, logToCode);
	
	// Check if logToCode is set to file
	if(compareString(logToCode, "file") != STR_EQ)
	{		
		// Start the timer
			// function: accessTimer
		timer->timerAccess = accessTimer(ZERO_TIMER, timer->timerString);	
		
		// Display System Start
			// function: printf
		printf("  %lf, OS: System Start\n", timer->timerAccess);

		// Lap timer
			// function: accessTimer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		// Display PCB initialization and create PCB
			// functio: printf, createPCB
		printf("  %lf, OS: Create Process Control Blocks\n", timer->timerAccess);
		createPCB(data, &timeLeft, pcb, localProcess);
		
		// Lap timer
			// function: accessTimer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		// Initialize processes to new state.
		localProcess->state = NEW;
		localProcess->processNumber = 0;
		printf("  %lf, OS: All processes initialized in New state\n", timer->timerAccess);
		
		// Lap timer
			// function: accessTimer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		// Change Process state to READY state
		localProcess->state = READY;
		printf("  %lf, OS: All processes now set in Ready state\n", timer->timerAccess);
		
		// Lap timer
			// function: accessTimer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		printf("  %lf, OS: Process %d selected with %d ms remaining\n", timer->timerAccess, localProcess->processNumber, timeLeft);
		
		// Lap timer
			// function: accessTimer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		// Change the process to RUNNING state
		localProcess->state = RUNNING;
		printf("  %lf, OS: Process %d set in RUNNING state\n\n", timer->timerAccess, localProcess->processNumber);
		
		// Run the process
			//function: runProcess
		runProcess(localProcess, processDataPtr, timer);
		
		// Lap timer
			// function: accessTimer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		//  Change the state to EXIT
		localProcess->state = EXIT;
		printf("\n  %lf, OS: Process %d ended and set in EXIT state\n", timer->timerAccess, localProcess->processNumber);
		
		// Stop the timer
			// function: accessTimer
		timer->timerAccess = accessTimer(STOP_TIMER, timer->timerString);

		printf("  %lf, OS: System stop\n", timer->timerAccess);
		
		printf("\nEnd Simulation - Complete\n");
		printf("=========================\n");
	}
	
	// EXIT
	fclose(filePtr);
	
	free(timer);
	free(localProcess);
	free(data);
	
	clearProcessDataList(processDataPtr);
	clearPCBlist(pcb);
	
}

/*
Function Name: createPCB
Algorithm: creates the PCB and calculates the total runtime.
Precondition: Two pointers, one to the config data and the other to
			  the first meta data op code.
Postcondition: Returns the calculated run time based on config data and meta data
Exceptions: none
Notes: assumes memory access/availability
*/
void createPCB(Data *data, int *totalTime, PCB *pcb, Process *localProcess)
{
	int processTime;
	int cycleRate;
	
	PCB *localPCBptr;
	
	localPCBptr = (PCB *) malloc(sizeof(PCB));
	
	while(data->mdDataPtr != NULL)
	{
		if(data->mdDataPtr->opLtr != 'S'
		   && data->mdDataPtr->opLtr != 'A')
		{
			// Determine the cycle rate for that process.
			switch(data->mdDataPtr->opLtr)
			{
				case 'I':
				case 'O':
					cycleRate = data->configDataPtr->ioCycleRate;
					break;
				case 'P':
					cycleRate = data->configDataPtr->procCycleRate;
					break;
				default:
					cycleRate = 0;
					break;
			}
			
			processTime = cycleRate * data->mdDataPtr->opValue;
			
			
			*totalTime += processTime;
			
			// Add data to the pcb
				//function: addPCBnode
			localPCBptr->opLtr = data->mdDataPtr->opLtr;
			copyString(localPCBptr->opName, data->mdDataPtr->opName);
			localPCBptr->processTime = processTime;
			localPCBptr->next = NULL;
			
			pcb = addPCBnode(pcb, localPCBptr);

			if(localProcess->currentProcess == NULL)
			{
				localProcess->currentProcess = pcb;
			}			
		}
		
		data->mdDataPtr = data->mdDataPtr->next;
	}

	free(localPCBptr);
}

/*
Function Name: runProcess
Algorithm: runs the op code for designated runtime and adds data to 
	       process data structure. If monitorFlag is set to true, the data
		   is also printed to the monitor.
Precondition: op code type, config data type, monitorFlag, timerAccess, and
			  location of last process data in linked list
Postcondition: assigns new process data structure node to end of linked list
			   and returns new node.
Exceptions: none
Notes: assumes memory access/availability

mdDataPtr runProcess(ConfigDataType *configDataPtr, Boolean monitorFlag, Timer *timer, ProcessData *localHeadPtr)
{
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	return mdDataPtr->next;	
}*/
void runProcess(Process *process, ProcessData *processData, Timer *timer)
{
	PCB *currentProcess;
	char operationType[10]; // operation, input, or output
	
	while(process->currentProcess != NULL)
	{
		currentProcess = process->currentProcess;
		
		switch(currentProcess->opLtr)
		{
			case 'I':
				copyString(operationType, "input");
				break;
			case 'O':
				copyString(operationType, "output");
				break;
			case 'P':
				copyString(operationType, "operation");
				break;
		}
		
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		printf("  %lf, Process: %d, %s %s start\n", timer->timerAccess, process->processNumber, 
													currentProcess->opName, operationType);
		runTimer(currentProcess->processTime);
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		printf("  %lf, Process: %d, %s %s end\n", timer->timerAccess, process->processNumber, 
													currentProcess->opName, operationType);
		process->currentProcess = currentProcess->next;
	}
}

/*
Function Name: logToFile
Algorithm: Does the same thing as runProcess but doesn't print to screen.
Precondition: data structure containing config data and meta data,
			  an int to store the time, a pcb struct to store the pcb,
			  Process and 
Postcondition: assigns new structure node to beginning of linked list
			   or end of an established linked listunchanged
Exceptions: none
Notes: assumes memory access/availability
*/
//void logToFile(ProcessData *processDataPtr, Timer *timer)


/*
Function Name: addNode
Algorithm: adds process structure with data to a linked list
Precondition: linked list pointer assigned to null or to one process link,
			  struct pointer assigned to process struct data
Postcondition: assigns new structure node to beginning of linked list
			   or end of an established linked listunchanged
Exceptions: none
Notes: assumes memory access/availability
*/
PCB *addPCBnode(PCB *localPtr, PCB *newNode)
{
	// check for local pointer assigned to null
	if(localPtr == NULL)
	{	
		// access memory for new link/node
			// function: malloc
		localPtr = (PCB *)malloc(sizeof(PCB));
		
		// assign all three values to newly created node
		// assign next pointer to null
			// function: copyString
		localPtr->processTime = newNode->processTime;
		localPtr->opLtr = newNode->opLtr;
		copyString(localPtr->opName, newNode->opName);
		localPtr->next = NULL;
			
		// return current local pointer
		return localPtr;
	}
	
	// assume end of list not found yet
	// assign recursive function to current's next link
		// function: addNode
	localPtr->next = addPCBnode(localPtr->next, newNode);
	
	// return current local pointer
	return localPtr;
}

/*
Function Name: addNode
Algorithm: adds process structure with data to a linked list
Precondition: linked list pointer assigned to null or to one process link,
			  struct pointer assigned to process struct data
Postcondition: assigns new structure node to beginning of linked list
			   or end of an established linked listunchanged
Exceptions: none
Notes: assumes memory access/availability
*/
ProcessData *addProcessNode(ProcessData *localPtr, ProcessData *newNode)
{
	// check for local pointer assigned to null
	if(localPtr == NULL)
	{	
		// access memory for new link/node
			// function: malloc
		localPtr = (ProcessData *)malloc(sizeof(ProcessData));
		
		// assign all three values to newly created node
		// assign next pointer to null
			// function: copyString
		localPtr->time = newNode->time;
		copyString(localPtr->process, newNode->process);
		localPtr->opLtr = newNode->opLtr;
		copyString(localPtr->opName, newNode->opName);
		localPtr->next = NULL;
			
		// return current local pointer
		return localPtr;
	}
	
	// assume end of list not found yet
	// assign recursive function to current's next link
		// function: addNode
	localPtr->next = addProcessNode(localPtr->next, newNode);
	
	// return current local pointer
	return localPtr;
}

/*
Function Name: clearProcessDataList
Algorithm: recursively iterates through process data linked list,
		   returns memory to OS from the bottom of the list upward
Precondition: linked list, with or without data
Postcondition: all node memory, if any, is returned to OS
			   return pointer (head) is set to null
Exceptions: none
Notes: none
*/
ProcessData *clearProcessDataList(ProcessData *localPtr)
{
	// check for local pointer not set to null (list not empty)
	if(localPtr != NULL)
	{		
		// check for local pointer's next node not null
		if(localPtr->next != NULL)
		{		
			// call recursive function with next pointer
				// function: clearMetaDataList
			clearProcessDataList(localPtr->next);
		}
		
		// after recursive call, release memory to OS
			// function: free
		free(localPtr);
	}
	
	// return null to calling function
	return NULL; 
}

/*
Function Name: clearPCBist
Algorithm: recursively iterates through pcb linked list,
		   returns memory to OS from the bottom of the list upward
Precondition: linked list, with or without data
Postcondition: all node memory, if any, is returned to OS
			   return pointer (head) is set to null
Exceptions: none
Notes: none
*/
PCB *clearPCBlist(PCB *localPtr)
{
	// check for local pointer not set to null (list not empty)
	if(localPtr != NULL)
	{		
		// check for local pointer's next node not null
		if(localPtr->next != NULL)
		{		
			// call recursive function with next pointer
				// function: clearMetaDataList
			clearPCBlist(localPtr->next);
		}
		
		// after recursive call, release memory to OS
			// function: free
		free(localPtr);
	}
	
	// return null to calling function
	return NULL; 
}






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
	char printString[STD_STR_LEN];
	FILE *filePtr;
	
	Boolean MONITOR_FLAG = True; // Default print to monitor
	Boolean LOG_TO_FILE = True; // Default log to file
	
	PCB *pcb = NULL;
	Process *localProcess;	
	ProcessData *processDataPtr;
	Timer *timer;
	Data *data;
	
	// allocate memory for the temporary data structures
		// function: malloc	
	timer = (Timer *) malloc(sizeof(Timer));
	localProcess = (Process *) malloc(sizeof(Process));
	data = (Data *) malloc(sizeof(Data));
	processDataPtr = (ProcessData *) malloc(sizeof(ProcessData));
	
	// Set the currentProcess to be NULL
	localProcess->currentProcess = NULL;
	localProcess->next = NULL;
	
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
	if(compareString(logToCode, "file") == STR_EQ)
	{	
		MONITOR_FLAG = False;
	}
	else if(compareString(logToCode, "monitor") == STR_EQ)
	{
		LOG_TO_FILE = False;
	}
	
	// Display Title
		// funciton: printf
	printf("\n================\n");
	printf("Begin Simulation\n\n");
	
	snprintf(printString, STD_STR_LEN, "Begin Simulation\n\n");
	copyString(processDataPtr->data, printString);
	processDataPtr->next = NULL;
	data->processDataStart = processDataPtr;
	processDataPtr = addProcessDataNode(data->processDataStart, processDataPtr);
	
	// Start the timer
		// function: accessTimer
	timer->timerAccess = accessTimer(ZERO_TIMER, timer->timerString);	
	
	// Display System Start
		// function: printf
	snprintf(printString, STD_STR_LEN, "  %lf, OS: System Start\n", timer->timerAccess);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Lap timer
		// function: accessTimer
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	
	// Display PCB initialization
		// functio: printf
	snprintf(printString, STD_STR_LEN, "  %lf, OS: Create Process Control Blocks\n", timer->timerAccess);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Create the PCB
		// function: createPCB
	pcb = createPCB(data, pcb, localProcess);
	
	// Lap timer
		// function: accessTimer
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	
	// Initialize processes to new state.
		// function: setNew
	snprintf(printString, STD_STR_LEN, 
				"  %lf, OS: All processes initialized in New state\n", 
				timer->timerAccess);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Lap timer
		// function: accessTimer
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	
	// Change Process state to READY state
		// function: setReady
	snprintf(printString, STD_STR_LEN, 
				"  %lf, OS: All processes now set in Ready state\n", 
				timer->timerAccess);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Lap timer
		// function: accessTimer
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	
	snprintf(printString, STD_STR_LEN, 
				"  %lf, OS: Process %d selected with %d ms remaining\n", 
				timer->timerAccess, localProcess->processNumber, localProcess->timeLeft);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Lap timer
		// function: accessTimer
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	
	// Change the process to RUNNING state
	localProcess->state = RUNNING;
	snprintf(printString, STD_STR_LEN, 
				"  %lf, OS: Process %d set in RUNNING state\n\n", 
				timer->timerAccess, localProcess->processNumber);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Run the process
		//function: runProcess
	runProcess(localProcess, processDataPtr, timer, MONITOR_FLAG);
	
	// Lap timer
		// function: accessTimer
	timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
	
	//  Change the state to EXIT
	localProcess->state = EXIT;
	snprintf(printString, STD_STR_LEN, 
				"\n  %lf, OS: Process %d ended and set in EXIT state\n", 
				timer->timerAccess, localProcess->processNumber);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	// Stop the timer
		// function: accessTimer
	timer->timerAccess = accessTimer(STOP_TIMER, timer->timerString);

	snprintf(printString, STD_STR_LEN, "  %lf, OS: System stop\n", timer->timerAccess);
	
	// Check for monitor flag and print to screen
	if(MONITOR_FLAG == True)
	{
		printf(printString);
	}
	
	// Add the data to the struct for file log
	copyString(processDataPtr->data, printString);
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	copyString(processDataPtr->data, "\nEnd Simulation - Complete\n");
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	copyString(processDataPtr->data, "=========================\n");
	processDataPtr = addProcessDataNode(processDataPtr, processDataPtr);
	
	if(LOG_TO_FILE == True)
	{
		logToFile(filePtr, data);
	}
	
	printf("\nEnd Simulation - Complete\n");
	printf("=========================\n");
	
	// EXIT
	fclose(filePtr);
	
	processDataPtr = clearProcessDataList(processDataPtr);
	pcb = clearPCBlist(pcb);
	
	free(timer);
	free(localProcess);
	free(data);
	free(pcb);
}

/*
Function Name: createPCB
Algorithm: creates the PCB and calculates the total runtime.
Precondition: Two pointers, one to the config data and the other to
			  the first meta data op code.
Postcondition: Returns the pcb created for that process
Exceptions: none
Notes: assumes memory access/availability
*/
PCB *createPCB(Data *data, PCB *pcb, Process *localProcess)
{
	// initialize function/variables
	int processTime;
	int cycleRate;
	
	PCB *newPCBptr;
	Process *newProcess;
	
	newPCBptr = (PCB *) malloc(sizeof(PCB));
	newProcess = (Process *) malloc(sizeof(Process));
	
	localProcess->timeLeft = 0;
	
	// Loop through meta data
	while(data->mdDataPtr != NULL)
	{
		if(data->mdDataPtr->opLtr == 'A')
		{
			// Check if application end
			if(compareString(data->mdDataPtr->opName, "end") == STR_EQ)
			{
				// free the temp memory
				free(newPCBptr);
				free(newProcess);
				
				return pcb;
			}
				
		}
		else if(data->mdDataPtr->opLtr != 'S')
		{
			// Determine the cycle rate for that process
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
			
			// Calculate the process time
			processTime = cycleRate * data->mdDataPtr->opValue;
			
			// Add the time to the processTime
			localProcess->timeLeft += processTime;
			
			// Add data to the pcb
				//function: addPCBnode
			newPCBptr->opLtr = data->mdDataPtr->opLtr;
			copyString(newPCBptr->opName, data->mdDataPtr->opName);
			newPCBptr->processTime = processTime;
			newPCBptr->next = NULL;
			
			pcb = addPCBnode(pcb, newPCBptr);
			
			// Store the first pcb into the process
			if(localProcess->currentProcess == NULL)
			{
				localProcess->currentProcess = pcb;
			}			
		}
		
		// go to next data
		data->mdDataPtr = data->mdDataPtr->next;
	}
	
	free(newPCBptr);
	free(newProcess);
	
	return pcb;
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
*/
SimCodeMessages runProcess(Process *process, ProcessData *processData, Timer *timer, Boolean MONITOR_FLAG)
{
	// initialize funciton/variables
	PCB *currentProcess;
	ProcessData *localDataPointer;
	char operationType[10]; // operation, input, or output
	char printString[STD_STR_LEN];
	
	// initialize POSIX thread
	pthread_t ioThread;
	struct arguments *arg;
	
	// Allocate memory for data storage
		// function: malloc
	localDataPointer = (ProcessData *)malloc(sizeof(ProcessData));
	
	// Loop through the process
	while(process->currentProcess != NULL)
	{
		// Skip the first process because it is a placeholder
		currentProcess = process->currentProcess;
		
		// Check for type of process
		switch(currentProcess->opLtr)
		{
			// Input must create POSIX thread
			case 'I':
				copyString(operationType, "input");
				if (pthread_create(&ioThread, NULL, ioProcess, (void *)arg)) 
				{
					return PTHREAD_ERROR;
				}
				if (pthread_join(ioThread, NULL)) 
				{
					fprintf(stderr,"Error while joining with child thread\n");
					exit(1);
				}
				break;
			// Output must create POSIX thread
			case 'O':
				copyString(operationType, "output");
				if (pthread_create(&ioThread, NULL, ioProcess, (void *)arg)) 
				{
					return PTHREAD_ERROR;
				}
				if (pthread_join(ioThread, NULL)) 
				{
					fprintf(stderr,"Error while joining with child thread\n");
					exit(1);
				}
				break;
			// Operation, store to operationType
			case 'P':
				copyString(operationType, "operation");
				break;
		}
		
		// Lapt timer
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		snprintf(printString, STD_STR_LEN, "  %lf, Process: %d, %s %s start\n", timer->timerAccess, process->processNumber, 
																			    currentProcess->opName, operationType);
		// Check for log to monitor
		if(MONITOR_FLAG == True)
		{
			printf(printString);
		}
		
		// Store data in linked list
		copyString(localDataPointer->data, printString);
		processData = addProcessDataNode(processData, localDataPointer);
		
		// Run timer for alloted time and lap.
		runTimer(currentProcess->processTime);
		timer->timerAccess = accessTimer(LAP_TIMER, timer->timerString);
		
		snprintf(printString, STD_STR_LEN, "  %lf, Process: %d, %s %s end\n", timer->timerAccess, process->processNumber, 
																			  currentProcess->opName, operationType);
		// Check for log to monitor																	  
		if(MONITOR_FLAG == True)
		{
			printf(printString);
		}
		
		// Store data in linked list
		copyString(localDataPointer->data, printString);
		processData = addProcessDataNode(processData, localDataPointer);
		
		// Step to next process in linked list.
		process->currentProcess = currentProcess->next;
	}
	
	// Free memory and exit success
	free(localDataPointer);
	
	
	return PROCESS_COMPLETE;
}


/*
Function Name: ioProcess
Algorithm: Does nothing right now
Precondition: A struct of arguments
Postcondition: none
Exceptions: none
Notes: none
*/
void *ioProcess(void *arg)
{
	return NULL;
}

/*
Function Name: logToFile
Algorithm: Takes the created data and stores it into the designated file.
Precondition: An open file pointer and a linked list of data to be stored.
Postcondition: Assumes the file pointer is already open.
Exceptions: none
Notes: none
*/
void logToFile(FILE *filePtr, Data *data)
{
	// initialize function/variables
	ProcessData *processDataPtr = data->processDataStart;
	ConfigDataType *configDataPtr = data->configDataPtr;
	
	
	// Display the config data at top of file.
		// function: displayConfigData
	displayConfigData(configDataPtr, filePtr);
	
	// Loop through the linked list
	while(processDataPtr != NULL)
	{
		// Print the data to the file
			// function: fprintf
		fprintf(filePtr, processDataPtr->data);
		
		// Move to the item in the linked list
		processDataPtr = processDataPtr->next;
	}
}


/*
Function Name: addPCBnode
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
Function Name: addProcessDataNode
Algorithm: adds process structure with data to a linked list
Precondition: linked list pointer assigned to null or to one process link,
			  struct pointer assigned to process struct data
Postcondition: assigns new structure node to beginning of linked list
			   or end of an established linked listunchanged
Exceptions: none
Notes: assumes memory access/availability
*/
ProcessData *addProcessDataNode(ProcessData *localPtr, ProcessData *newNode)
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
		copyString(localPtr->data, newNode->data);
		localPtr->next = NULL;
			
		// return current local pointer
		return localPtr;
	}
	
	// assume end of list not found yet
	// assign recursive function to current's next link
		// function: addNode
	localPtr->next = addProcessDataNode(localPtr->next, newNode);
	
	// return current local pointer
	return localPtr;
}

/*
Function Name: addProcessNode
Algorithm: adds process structure with data to a linked list
Precondition: linked list pointer assigned to null or to one process link,
			  struct pointer assigned to process struct data
Postcondition: assigns new structure node to beginning of linked list
			   or end of an established linked listunchanged
Exceptions: none
Notes: assumes memory access/availability
*/
Process *addProcessNode(Process *localPtr, Process *newNode)
{
	// initialize funciton/variables
	int nextProcessNum = localPtr->processNumber + 1;
	
	// check for local pointer assigned to null
	if(localPtr == NULL)
	{	
		// access memory for new link/node
			// function: malloc
		localPtr = (Process *)malloc(sizeof(Process));
		
		// assign all three values to newly created node
		// assign next pointer to null
			// function: copyString
		int processNumber = nextProcessNum;
		localPtr->state = NEW;
		localPtr->timeLeft = 0;
		localPtr->currentProcess = NULL;
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

/*
Function Name: clearProcess
Algorithm: recursively iterates through process linked list,
		   returns memory to OS from the bottom of the list upward
Precondition: linked list, with or without data
Postcondition: all node memory, if any, is returned to OS
			   return pointer (head) is set to null
Exceptions: none
Notes: none
*/
Process *clearProcess(Process *localPtr)
{
	// check for local pointer not set to null (list not empty)
	if(localPtr != NULL)
	{		
		// check for local pointer's next node not null
		if(localPtr->next != NULL)
		{		
			// call recursive function with next pointer
				// function: clearMetaDataList
			clearProcess(localPtr->next);
		}
		
		// after recursive call, release memory to OS
			// function: free
		free(localPtr);
	}
	
	// return null to calling function
	return NULL; 
}






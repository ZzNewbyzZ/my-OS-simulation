// header files
#include <stdio.h>
#include "ConfigAccess.h"
#include "MetaDataAccess.h"
#include "Simulation.h"

/*
Function name: main
Algorithm: driver function to test config and metadata file
		   upload operation together
Precondition: none
Postcondition: returns zero (0) on success
Exceptions: none
Note: demonstrates use of combined files
*/
int main( int argc, char **argv )
{
    // initialize function/variables
	int configAccessResult;
	int mdAccessResult;
	char configFileName[MAX_STR_LEN];
	char mdFileName[MAX_STR_LEN];
	ConfigDataType *configDataPtr;
	OpCodeType *mdData;
    
    // display title
		// function: printf
	printf("\nSimulator Program\n");
	printf("=================\n\n");

    // check for not correct number of command line arguments (two)
	if(argc < 2)
	{

        // print missing command line argument error
			// function: printf
		printf("ERROR: Program requires file name for config file ");
		printf("as command line argument\n");
		printf("\nProgram Terminated\n");

        // return non-normal program result
		return 1;
	}
	
    // get data from configuration file
		// function: copyString, getConfigData
	copyString(configFileName, argv[1]);
	configAccessResult = getConfigData(configFileName, &configDataPtr);

    // check for successful upload
	if(configAccessResult == NO_ERR)
	{
        // display Configuration files upload
			// function: printf
		printf("Uploading Configuration Files\n");
	}
    // otherwise, assume failed upload
	else
	{

        // display configuration upload error
			// function: displayConfigError
		displayConfigError(configAccessResult);
		
		// shut down program

        // clear configuration data
			// function: clearConfigData
		clearConfigData(&configDataPtr);

        // add endline for vertical spacing
			// function: printf
		printf("\n");
    
        // return program success
        return 0;		
	}
	
	// get data from meta data file
		// function: copyString, getOpCodes
	copyString(mdFileName, configDataPtr->metaDataFileName);
	mdAccessResult = getOpCodes(mdFileName, &mdData);
	
	// check for successful upload
	if(mdAccessResult == NO_ERR)
	{	
		// display Meta Data files upload
			// function: printf
		printf("\nUploading Meta Data Files\n");
	}	
	
	// otherwise, assume unsuccessful upload
	else
	{
		// display meta data error message
			//function: displayMetaDataError
		displayMetaDataError(mdAccessResult);
		
		// shut down program

        // clear configuration data
			// function: clearConfigData
		clearConfigData(&configDataPtr);
		// clear meta data
			// function: clearMetaDataList
		mdData = clearMetaDataList(mdData);

        // add endline for vertical spacing
			// function: printf
		printf("\n");
    
        // return program success
        return 0;
	}
	
	// Begin the simulation
		// function: startSimulation
	runSimulation(configDataPtr, mdData);
	
	
	
    // shut down program

        // clear configuration data
			// function: clearConfigData
		clearConfigData(&configDataPtr);
		// clear meta data
			// function: clearMetaDataList
		mdData = clearMetaDataList(mdData);

        // add endline for vertical spacing
			// function: printf
		printf("\n");
    
        // return program success
        return 0;
}
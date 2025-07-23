//***************************** File Operation *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : Device_manager.c
// Summary : Contains all File operations needed for code execution
// Note    : None
// Author  : Bimesh Raj K R
// Date    : 23/Jul/2025
//******************************************************************************

//****************************** Include Files *********************************
#include "fileOperations.h"

//******************************* Local Types **********************************

//***************************** Local Constants ********************************

//***************************** Local Variables ********************************

//***************************** Local Functions ********************************

//**************************.fileOperationOpen.*********************************
// Purpose : Function to Open a File
// Inputs  : **ppstFilePointer - a pointer to the file to be opened
//           *cFileName - Contains the name of the file
//           *cMode - Contains the file access mode
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationOpen(FILE **ppstFilePointer, int8 *cFileName, int8 *pcMode)
{
    bool blCheck = false;

    if (NULL != ppstFilePointer)
    {
        *ppstFilePointer = fopen(cFileName, pcMode);

        if (NULL != *ppstFilePointer)
        {
            blCheck = true;
        }
        else
        {
            if(0 != strcmp("r",pcMode))
            {
                printf("File cannot be opened\n");
            }
        }
    }
    else
    {
        printf("Null check failed\n");
    }

    return blCheck;
}

//**************************.fileOperationClose.********************************
// Purpose : Function to Close a File
// Inputs  : **ppstFilePointer - a pointer to the file to be closed
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationClose(FILE **ppstFilePointer)
{
   bool blCheck = false;

    if (NULL != ppstFilePointer)
    {
        if (NULL != *ppstFilePointer)
        {
            fclose(*ppstFilePointer);
            blCheck = true;
        }
        else
        {
            printf("File cannot be closed\n");
        }
    }
    else
    {
        printf("Null check failed\n");
    }

    return blCheck;
}

//************************.fileOperationGetSize.********************************
// Purpose : Function to Get the size of a File
// Inputs  : **ppstFilePointer - a pointer to the file to be opened
//           lOffset - Contains the offset value
//           cPosition - Starting point for the offset
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationGetSize(FILE **ppstFilePointer, 
                          int32 lOffset, 
                          int32 lPosition,
                          int32 *plFileSize)
{
    bool blCheck = false;

    if (NULL != ppstFilePointer)
    {
        if (NULL != *ppstFilePointer)
        {
            if (0 == fseek(*ppstFilePointer, lOffset, lPosition))
            {
                *plFileSize = ftell(*ppstFilePointer);

                if(-1L != *plFileSize)
                {
                    rewind(*ppstFilePointer);
                    blCheck = true;
                }
                else
                {
                    printf("ftell failed\n");
                }

            }
            else
            {
                printf("fseek failed\n");
            }
        }
        else
        {
            printf("File pointer points to Null\n");
        }
    }
    else
    {
        printf("Null check failed\n");
    }

    return blCheck;
}

//**************************.fileOperationRead.*********************************
// Purpose : Function to read a File
// Inputs  : **ppstFilePointer - a pointer to the file to be opened
//           lByteSize - Size of an element inside file in bytes
//           lFileSize - Number of elements inside file
//           *pcFileData - Points to the location to store data
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationRead(int8 *pcFileData,
                       int32 lByteSize,
                       int32 lFileSize,
                       FILE **ppstFilePointer)
{
    bool blCheck = false;
    int32 lReadSize = 0;

    if (NULL != ppstFilePointer)
    {
        if(NULL != pcFileData)
        {
            if (NULL != *ppstFilePointer)
            {
                lReadSize = fread(pcFileData, lByteSize, lFileSize, 
                    *ppstFilePointer);

                if (lReadSize == lFileSize)
                {
                    blCheck = true;
                }
                else
                {
                    printf("fread failed\n");
                }
            }
            else
            {
                printf("File pointer points to Null\n");
            }
        }
        else
        {
            printf("Data could not be found\n");
        }
    }
    else
    {
        printf("Null check failed\n");
    }

    return blCheck;
}

//**************************.fileOperationWrite.********************************
// Purpose : Function to write a string to a File
// Inputs  : **ppstFilePointer - a pointer to the file to be opened
//           *cString - points to the string to be put in the file
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationWrite(int8 *cString,
                        FILE **ppstFilePointer)
{
    bool blCheck = false;

    if (NULL != ppstFilePointer)
    {
        if(NULL != cString)
        {
            if (NULL != *ppstFilePointer)
            {
                if (0 <= fputs(cString, *ppstFilePointer))
                {
                    blCheck = true;
                }
                else
                {
                    printf("fputs failed\n");
                }
            }
            else
            {
                printf("File pointer points to Null\n");
            }
        }
        else
        {
            printf("Data could not be found\n");
        }
    }
    else
    {
        printf("Null check failed\n");
    }

    return blCheck;
}

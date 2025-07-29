//***************************** File Operation *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : fileOperations.c
// Summary : Contains all File operations needed for managing devices
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
// Inputs  : ppstFilePointer - Pointer to the file to be opened
//           cFileName - Contains the name of the file
//           pcMode - Pointer to the file access mode
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
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
            if(0 != strcmp(READ_MODE, pcMode))
            {
                printf("\tFile cannot be opened\n");
            }
        }
    }
    else
    {
        printf("\tNull check failed\n");
    }

    return blCheck;
}

//**************************.fileOperationClose.********************************
// Purpose : Function to Close a File
// Inputs  : ppstFilePointer - Pointer to the file to be closed
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
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
            printf("\tFile cannot be closed\n");
        }
    }
    else
    {
        printf("\tNull check failed\n");
    }

    return blCheck;
}

//************************.fileOperationGetSize.********************************
// Purpose : Function to Get the size of a File
// Inputs  : ppstFilePointer - Pointer to the file to be opened
//           ulOffset - Contains the number of bytes the file pointer needs 
//           to be moved
//           ulPosition - Starting point for the offset
//           pulFileSize - Pointer to the position of the File pointer with 
//           respect to starting of the file
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationGetSize(FILE **ppstFilePointer, uint32 ulOffset, 
                          uint32 ulPosition, uint32 *pulFileSize)
{
    bool blCheck = false;

    if (NULL != ppstFilePointer)
    {
        if (NULL != *ppstFilePointer)
        {
            if (0 == fseek(*ppstFilePointer, ulOffset, ulPosition))
            {
                *pulFileSize = ftell(*ppstFilePointer);

                if(-1UL != *pulFileSize)
                {
                    rewind(*ppstFilePointer);
                    blCheck = true;
                }
                else
                {
                    printf("\tftell failed\n");
                }

            }
            else
            {
                printf("\tfseek failed\n");
            }
        }
        else
        {
            printf("\tFile pointer points to Null\n");
        }
    }
    else
    {
        printf("\tNull check failed\n");
    }

    return blCheck;
}

//**************************.fileOperationRead.*********************************
// Purpose : Function to read a File
// Inputs  : ppstFilePointer - Pointer to the file to be opened
//           ulByteSize - Size of an element inside file in bytes
//           ulFileSize - Number of elements inside file
//           pcFileData - Pointer to the location to store data
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool fileOperationRead(int8 *pcFileData, uint32 ulByteSize,
                       uint32 ulFileSize, FILE **ppstFilePointer)
{
    bool blCheck = false;
    uint32 lReadSize = 0;

    if (NULL != ppstFilePointer)
    {
        if(NULL != pcFileData)
        {
            if (NULL != *ppstFilePointer)
            {
                lReadSize = fread(pcFileData, ulByteSize, ulFileSize, 
                    *ppstFilePointer);

                if (lReadSize == ulFileSize)
                {
                    blCheck = true;
                }
                else
                {
                    printf("\tfread failed\n");
                }
            }
            else
            {
                printf("\tFile pointer points to Null\n");
            }
        }
        else
        {
            printf("\tData could not be found\n");
        }
    }
    else
    {
        printf("\tNull check failed\n");
    }

    return blCheck;
}

//**************************.fileOperationWrite.********************************
// Purpose : Function to write a string to a File
// Inputs  : ppstFilePointer - Pointer to the file to be opened
//           cString - Pointer to the string to be put in the file
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
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
                    printf("\tfputs failed\n");
                }
            }
            else
            {
                printf("\tFile pointer points to Null\n");
            }

            cJSON_free(cString);
        }
        else
        {
            printf("\tData could not be found\n");
        }
    }
    else
    {
        printf("\tNull check failed\n");
    }

    return blCheck;
}

//EOF
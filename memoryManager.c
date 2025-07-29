//***************************** Memory Manager *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : memoryManager.c
// Summary : Allocates and frees memory for device operations.
// Note    : None
// Author  : Bimesh Raj K R
// Date    : 23/Jul/2025
//******************************************************************************

//****************************** Include Files *********************************
#include "memoryManager.h"

//******************************* Local Types **********************************

//***************************** Local Constants ********************************

//***************************** Local Variables ********************************

//***************************** Local Functions ********************************

//**************************.memoryManagerAllocate.*****************************
// Purpose : Function to allocate memory
// Inputs  : pMemory - The location where memory needs to be allocated
//           ulSize - The size of the memory
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool memoryManagerAllocate(void **pMemory, uint32 ulSize)
{
    bool blCheck = false;

    if ((0 != ulSize) && (NULL != pMemory))
    {
        *pMemory = malloc(ulSize);
        blCheck = true;
    }
    else
    {
        printf("\tMalloc is not possible\n");
    }

    return blCheck;
}

//****************************.memoryManagerFree.*******************************
// Purpose : Function to free memory
// Inputs  : pMemory - The memory that needs to be freed
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool memoryManagerFree(void **pMemory)
{
    bool blCheck = false;

    if ((NULL != *pMemory) && (NULL != pMemory))
    {
        free(*pMemory);
        blCheck = true;
    }
    else
    {
        printf("\tUnable to free memory\n");
    }

    return blCheck;
}

//EOF
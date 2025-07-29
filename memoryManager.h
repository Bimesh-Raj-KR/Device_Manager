//********************************* Device Manager *****************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved 
//******************************************************************************
//
// Summary : Contain all forward declarations required for device management in 
//           memoryManager.c functions
// Note    : None
// 
//******************************************************************************
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

//******************************* Include Files ********************************
#include "common.h"

//******************************* Global Types *********************************

//***************************** Global Constants *******************************

//***************************** Global Variables *******************************

//**************************** Forward Declarations ****************************
bool memoryManagerAllocate(void **pMemory, uint32 ulSize);
bool memoryManagerFree(void **pMemory);

//*********************** Inline Method Implementations ************************ 

#endif // MEMORY_MANAGER_H
// EOF 
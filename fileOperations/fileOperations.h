//********************************* Device Manager *****************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved 
//******************************************************************************
//
// Summary : Contain all forward declarations required for 
//           fileOperation.c functions
// Note    : None
// 
//******************************************************************************
#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

//******************************* Include Files ********************************
#include "common.h"

//******************************* Global Types *********************************

//***************************** Global Constants *******************************

//***************************** Global Variables *******************************

//**************************** Forward Declarations ****************************
bool fileOperationOpen(FILE **ppstFilePointer, int8 *cFileName, int8 *pcMode);
bool fileOperationClose(FILE **ppstFilePointer);
bool fileOperationGetSize(FILE **ppstFilePointer, uint32 ulOffset, 
                          uint32 ulPosition, uint32 *pulFileSize);
bool fileOperationRead(int8 *pcFileData, uint32 ulByteSize,
                       uint32 ulFileSize, FILE **ppstFilePointer);
bool fileOperationWrite(int8 *cString, FILE **ppstFilePointer);

//*********************** Inline Method Implementations ************************ 

#endif // FILE_OPERATIONS_H
// EOF 
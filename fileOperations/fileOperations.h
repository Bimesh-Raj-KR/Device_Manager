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
bool fileOperationGetSize(FILE **ppstFilePointer, 
                          int32 lOffset, 
                          int32 cPosition,
                          int32 *plFileSize);
bool fileOperationRead(int8 *pcFileData,
                       int32 lByteSize,
                       int32 lFileSize,
                       FILE **ppstFilePointer);
bool fileOperationWrite(int8 *cString,
                        FILE **ppstFilePointer);

//*********************** Inline Method Implementations ************************ 

#endif // FILE_OPERATIONS_H
// EOF 
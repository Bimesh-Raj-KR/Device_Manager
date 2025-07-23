//********************************* Device Manager *****************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved 
//******************************************************************************
//
// Summary : Contain all type definitions, global constants that defines number 
//           of arithmetic operations and forward declarations required for 
//           deviceManager.c functions
// Note    : None
// 
//******************************************************************************
#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

//******************************* Include Files ********************************
#include "common.h"
#include "fileOperations.h"

//******************************* Global Types *********************************
// Defines Memory declarations
typedef short int16;

//Defines the linked List
typedef struct DEVICE_MANAGER
{
    int32 lDeviceId;
    int32 lVendorId;
    int8 Name[MAX_NAME_SIZE];
    int32 lState;
    struct DEVICE_MANAGER *pstNext;
}DEVICE_MANAGER;

//Defines the menu and Function pointer
typedef struct _MENU_
{
    const int8 Menu[MAX_MENU_SIZE];
    bool (*pManager)(DEVICE_MANAGER**);
}MENU;

//***************************** Global Constants *******************************
#define DIVISION_OPERATION      (3)
#define MAX_MENU_COUNT          (4)

//***************************** Global Variables *******************************

//**************************** Forward Declarations ****************************
bool deviceManagerAdd(DEVICE_MANAGER **ppstHead);
bool deviceManagerDelete(DEVICE_MANAGER **ppstHead);
bool deviceManagerSearch(DEVICE_MANAGER **ppstHead);
bool deviceManagerDisplay(DEVICE_MANAGER **ppstHead);
bool deviceManagerSelect( MENU *stDevice);

//*********************** Inline Method Implementations ************************ 

#endif // DEVICE_MANAGER_H
// EOF 
//********************************* Device Manager *****************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved 
//******************************************************************************
//
// Summary : Contain all type definitions, global constants and forward 
//           declarations required for device management in 
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
// Defines the variable declaration
typedef unsigned char uint8;

//Defines the linked List
typedef struct DEVICE_MANAGER
{
    uint32 ulDeviceId;
    uint32 ulVendorId;
    int8 cName[MAX_NAME_SIZE];
    uint16 unState;
    struct DEVICE_MANAGER *pstNext;
}DEVICE_MANAGER;

//Defines the menu and Function pointer
typedef struct DEVICE_MENU_
{
    const int8 Menu[MAX_MENU_SIZE];
    bool (*pManager)(DEVICE_MANAGER**);
}DEVICE_MENU;

//***************************** Global Constants *******************************
#define DIVISION_OPERATION      (3)
#define MAX_MENU_COUNT          (4)
#define FILE_NAME               ("device.json")
#define WRITE_MODE              ("w")
#define NULL_CHARACTER          ('\0')

//***************************** Global Variables *******************************

//**************************** Forward Declarations ****************************
bool deviceManagerAdd(DEVICE_MANAGER **ppstHead);
bool deviceManagerDelete(DEVICE_MANAGER **ppstHead);
bool deviceManagerSearch(DEVICE_MANAGER **ppstHead);
bool deviceManagerDisplay(DEVICE_MANAGER **ppstHead);
bool deviceManagerSelect( DEVICE_MENU *stDevice);

//*********************** Inline Method Implementations ************************ 

#endif // DEVICE_MANAGER_H
// EOF 
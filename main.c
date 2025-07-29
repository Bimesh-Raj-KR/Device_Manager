//***************************** Device Manager *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : main.c
// Summary : Manages different devices. Add, delete, search and display devices 
//           using a json file and linked list
// Note    : None
// Author  : Bimesh Raj K R
// Date    : 23/Jul/2025
//******************************************************************************

//****************************** Include Files *********************************
#include "deviceManager.h"

//******************************* Local Types **********************************

//***************************** Local Constants ********************************

//***************************** Local Variables ********************************

//***************************** Local Functions ********************************

//***********************************.main.*************************************
// Purpose : Function to Manage device operations
// Inputs  : None
// Outputs : None
// Return  : 0
// Notes   : None
//******************************************************************************
int main()
{
    DEVICE_MENU stDevice[MAX_MENU_COUNT] = 
    {
        {"Add Device", deviceManagerAdd},
        {"Delete Device", deviceManagerDelete},
        {"Search Device", deviceManagerSearch},
        {"Display all device", deviceManagerDisplay},
        {"Exit", NULL}
    };

    if (true != deviceManagerSelect(stDevice))
    {
        printf("\tChoice Outside Range or Error Found\n");
    }

    return 0;
}

//EOF
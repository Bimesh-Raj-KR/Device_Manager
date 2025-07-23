//***************************** Device Manager *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : Device_manager.c
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
static bool deviceManagerFree(DEVICE_MANAGER **ppstHead);
static bool deviceManagerJson(DEVICE_MANAGER **pstCurrentNode);
static bool deviceManagerJsonRead(cJSON **pstJsonArray);
static bool deviceManagerJsonWrite(cJSON **pstJsonArray);
static bool deviceManagerJsonPrint(cJSON **pstJsonObject,
                            cJSON **pstDeviceDetails);


//**************************.deviceManagerAdd.**********************************
// Purpose : Function to Add a Device to list
// Inputs  : ppstHead - Points to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************

bool deviceManagerAdd(DEVICE_MANAGER **ppstHead)
{
    bool blCheck = false;
    int8 cChoice = 'y';
    int16 nIterator = 0;
    DEVICE_MANAGER *pstCurrentNode = NULL;
    DEVICE_MANAGER *pstNewNode = NULL;
    const int8 *pcState[MAX_MENU_COUNT] = {"Online", "Charging", 
                                         "Disabled", "Offline"};

    while (cChoice == 'y')
    {
        pstNewNode = malloc(sizeof(DEVICE_MANAGER));

        pstCurrentNode = *ppstHead;

        if (NULL != pstNewNode)
        {
            printf("\nEnter Device Id: ");
            scanf("%ld", &pstNewNode->lDeviceId);
            printf("\nEnter Vendor Id: ");
            scanf("%ld", &pstNewNode->lVendorId);
            printf("\nEnter Device Name: ");
            scanf("%s", pstNewNode->Name);
            printf("\n**********DEVICE STATE**********\n");
            for (nIterator = 0; nIterator < MAX_MENU_COUNT; nIterator ++)
            {
                printf("\t%s = %hd\n", pcState[nIterator], nIterator);
            }
            printf("*********************************\n\n");
            printf("\nEnter Device State: ");
            scanf("%ld", &pstNewNode->lState);

            if ((0 > pstNewNode->lState) || 
                (MAX_MENU_COUNT <= pstNewNode->lState))
            {
                printf("\n\tINVALID DEVICE STATE\n");
                free(pstNewNode);
                pstNewNode = NULL;

                break;
            }

            pstNewNode->pstNext = NULL;

            if(NULL == *ppstHead)
            {
                *ppstHead = pstNewNode;
            }
            else
            {
                pstCurrentNode = *ppstHead;

                while (pstCurrentNode->pstNext != NULL)
                {
                    pstCurrentNode = pstCurrentNode->pstNext;
                }
                pstCurrentNode->pstNext = pstNewNode;
                
            }

            deviceManagerJson(&pstNewNode);
            blCheck = true;
        }
        else
        {   
            printf("Memory Allocation Failed\n");

            break;
        }

        printf("\n***************ADD MENU*************\n");
        printf("\tPress 'y' to Continue\n\tPress 'n' to Stop\n");
        printf("*************************************\n\n");

        while(getchar() != '\n');

        scanf("%c", &cChoice);

        if (('y' != cChoice) && ('n' != cChoice))
        {
            printf("\tINVALID CHOICE\n");
        }
    }

    return blCheck;
}

//***************************.deviceManagerJson.********************************
// Purpose : Function to Add a Device Details to the json File
// Inputs  : pstCurrentNode - Points to the node currently made in the 
//           linked list
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerJson(DEVICE_MANAGER **pstCurrentNode)
{
    bool blCheck = false;
    int32 lFileSize = 0;
    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    FILE *pstFilePointer = NULL;
    int8 *pcJsonString = NULL;
    int8 *pcFileData = NULL;
    int8 *pcFileName = "device.json";
    int8 *cMode = "r";

    do
    {
        if (NULL != pstCurrentNode)
        {
            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                if (true != fileOperationGetSize(&pstFilePointer, 0, 
                                        SEEK_END, &lFileSize))
                {
                    printf("File size could not be determined\n");

                    break;
                }

                pcFileData = malloc(lFileSize + 1);

                if(NULL != pcFileData)
                {
                    if (true != fileOperationRead(pcFileData, 1, 
                                lFileSize, &pstFilePointer))
                    {
                        printf("File Cannot be Read\n");

                        break;
                    }

                    pcFileData[lFileSize] = '\0';
                    pstJsonArray = cJSON_Parse(pcFileData);
                    free(pcFileData);
                }
                else
                {
                    printf("Memory Allocation Failed");
                    fileOperationClose(&pstFilePointer);

                    break;
                }

                if(false == cJSON_IsArray(pstJsonArray))
                {
                    cJSON_Delete(pstJsonArray);
                    pstJsonArray = cJSON_CreateArray();
                }

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("Unable to close file\n");
                }
            }
            else
            {
                pstJsonArray = cJSON_CreateArray();
            }

            pstFilePointer = NULL;
            cMode = "w";

            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                pstJsonObject = cJSON_CreateObject();
                cJSON_AddNumberToObject(pstJsonObject, "Device ID", 
                    (*pstCurrentNode)->lDeviceId);
                cJSON_AddNumberToObject(pstJsonObject, "Vendor ID", 
                    (*pstCurrentNode)->lVendorId);
                cJSON_AddStringToObject(pstJsonObject, "Device Name", 
                    (*pstCurrentNode)->Name);
                cJSON_AddNumberToObject(pstJsonObject, "Device State", 
                    (*pstCurrentNode)->lState);
                cJSON_AddItemToArray(pstJsonArray, pstJsonObject);

                pcJsonString = cJSON_Print(pstJsonArray);

                if (true != fileOperationWrite(pcJsonString, 
                                &pstFilePointer))
                {
                    printf("Unable to write in file\n");
                    cJSON_free(pcJsonString);
                    blCheck = true;
                }

                cJSON_Delete(pstJsonArray);

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("Unable to close file\n");
                }

                break;
            }
            else
            {
                printf("Unable to open the file.\n");
                break;
            }
        }
        else
        {
            printf("Null Check failed\n");
            break;
        }
    }while(1);

    return blCheck;
}

//*************************.deviceManagerDelete.********************************
// Purpose : Function to Delete a Device from list
// Inputs  : ppstHead - Points to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerDelete(DEVICE_MANAGER **ppstHead)
{
    int32 lSearchDevice = 0;
    bool blCheck = false;
    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    cJSON *pstDeviceDetails = NULL;
    int32 lIterator = 0;

    printf("Enter The Device ID to Remove: ");
    scanf("%ld", &lSearchDevice);
    
    if (NULL != ppstHead)
    {
        do
        {
            if (true != deviceManagerJsonRead(&pstJsonArray))
            {
                printf("File doesn't exist\n");

                break;
            }

            if (true == cJSON_IsArray(pstJsonArray))
            {
                for (lIterator = 0; 
                        lIterator < cJSON_GetArraySize(pstJsonArray);
                        lIterator ++)
                {
                    pstJsonObject = cJSON_GetArrayItem(pstJsonArray, lIterator);
                    pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            pstJsonObject, "Device ID");

                    if((true == cJSON_IsNumber(pstDeviceDetails)) && 
                            (pstDeviceDetails->valueint == lSearchDevice))
                    {
                        cJSON_DeleteItemFromArray(pstJsonArray, lIterator);
                        printf("\n\tDEVICE REMOVED\n");
                        blCheck = true;
                    }
                }

                if ((lIterator == cJSON_GetArraySize(pstJsonArray)) && 
                                        (true != blCheck))
                {
                    printf("\n\tDEVICE NOT FOUND\n");
                }

                if (true != deviceManagerJsonWrite(&pstJsonArray))
                {
                    printf("Unable to write in the file\n");

                    blCheck = false;

                    break;
                }

                break;
            }

            printf("Invalid file format\n");

            break;

        }while (1);
    }
    else
    {
        printf("Null Check failed\n");
    }

    return blCheck;
}

//**************************.deviceManagerSearch.*******************************
// Purpose : Function to Search a Device from the list
// Inputs  : ppstHead - Points to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerSearch(DEVICE_MANAGER **ppstHead)
{
    int32 lSearchDevice = 0;
    bool blCheck = false;

    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    cJSON *pstDeviceDetails = NULL;
    int32 lIterator = 0;

    printf("Enter The Device ID to search: ");
    scanf("%ld", &lSearchDevice);

    if (NULL != ppstHead)
    {
        do
        {
            if (true != deviceManagerJsonRead(&pstJsonArray))
            {
                printf("File doesn't exist\n");

                break;
            }

            if (true == cJSON_IsArray(pstJsonArray))
            {
                for (lIterator = 0; 
                        lIterator < cJSON_GetArraySize(pstJsonArray);
                        lIterator ++)
                {
                    pstJsonObject = cJSON_GetArrayItem(pstJsonArray, lIterator);
                    pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            pstJsonObject, "Device ID");

                    if((true == cJSON_IsNumber(pstDeviceDetails)) && 
                            (pstDeviceDetails->valueint == lSearchDevice))
                    {
                        printf("\tDEVICE FOUND\n");

                        if (true == deviceManagerJsonPrint(&pstJsonObject, 
                                                        &pstDeviceDetails))
                        {
                            blCheck = true;
                        }
                        else
                        {
                            printf("The Device Details could not be printed\n");
                        }

                        break;
                    }
                }

                if ((lIterator == cJSON_GetArraySize(pstJsonArray)) && 
                            (true != blCheck))
                {
                    cJSON_Delete(pstJsonArray);

                    printf("\n\tDEVICE NOT FOUND\n");

                    break;
                }

                cJSON_Delete(pstJsonArray);

                break;
            }

            printf("Invalid file format\n");

            break;
        }while (1);
    }
    else
    {
        printf("Null Check failed\n");
    }

    return blCheck;
}

//**************************.deviceManagerDisplay.******************************
// Purpose : Function to Display all Devices
// Inputs  : ppstHead - Points to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerDisplay(DEVICE_MANAGER **ppstHead)
{
    bool blCheck = false;
    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    cJSON *pstDeviceDetails = NULL;
    int32 lIterator = 0;

    if (NULL != ppstHead)
    {
        do
        {
            if (true != deviceManagerJsonRead(&pstJsonArray))
            {
                printf("File doesn't exist\n");

                break;
            }

            if (true == cJSON_IsArray(pstJsonArray))
            {
                for (lIterator = 0; 
                        lIterator < cJSON_GetArraySize(pstJsonArray);
                        lIterator ++)
                {
                    pstJsonObject = cJSON_GetArrayItem(pstJsonArray, lIterator);

                    printf("\n\tDEVICE: %ld\n", lIterator + 1);
                    
                    if (true == deviceManagerJsonPrint(&pstJsonObject, 
                                                        &pstDeviceDetails))
                    {
                        blCheck = true;
                    }
                    else
                    {
                        printf("The Device Details could not be printed\n");

                        break;
                    }
                }

                cJSON_Delete(pstJsonArray);

            }
            else
            {
                printf("Invalid file format\n");;
            }

            break;

        }while (1);
    }
    else
    {
        printf("Null Check failed\n");
    }
    return blCheck;
}

//**************************.deviceManagerFree.*********************************
// Purpose : Function that frees up memory
// Inputs  : ppstHead - Points to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerFree(DEVICE_MANAGER **ppstHead)
{
    bool blCheck = false;
    DEVICE_MANAGER *pstCurrentNode = NULL;
    DEVICE_MANAGER *pstNext = NULL;

    if ((NULL != ppstHead) && (NULL != *ppstHead))
    {
        pstCurrentNode = *ppstHead;

        while (NULL != pstCurrentNode) 
        {
            pstNext = pstCurrentNode->pstNext;
            free(pstCurrentNode);
            pstCurrentNode = pstNext;
        }

        blCheck = true;
    }
    

    return blCheck;
}

//**************************.deviceManagerJsonRead.*****************************
// Purpose : Function to Read a Json file
// Inputs  : pstJsonArray - Points to the Object array inside Json file
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerJsonRead(cJSON **pstJsonArray)
{
    bool blCheck = false;
    int32 lFileSize = 0;
    int8 *pcFileData = NULL;
    FILE *pstFilePointer = NULL;
    int8 *pcFileName = "device.json";
    int8 *cMode = "r";
    int32 lOffset = 0;
    int32 lPosition = 0;

    do
    {
        if (NULL != pstJsonArray)
        {
            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                if (NULL == pstFilePointer)
                {
                    printf("Null Check Failed\n");

                    break;
                }

                lPosition = SEEK_END;

                if (true != fileOperationGetSize(&pstFilePointer, lOffset, 
                                            lPosition, &lFileSize))
                {
                    printf("File size could not be determined\n");

                    break;
                }

                pcFileData = malloc(lFileSize + 1);

                if (NULL != pcFileData)
                {
                    if (true != fileOperationRead(pcFileData, 1, 
                                lFileSize, &pstFilePointer))
                    {
                        printf("File Cannot be Read\n");

                        break;
                    }
                    pcFileData[lFileSize] = '\0';
                    *pstJsonArray = cJSON_Parse(pcFileData);
                    free(pcFileData);
                    blCheck = true;
                    
                }
                else
                {
                    printf("Memory Allocation Failed\n");
                }

                if (NULL == *pstJsonArray)
                {
                    printf("Unable to parse information\n");
                    cJSON_Delete(*pstJsonArray);
                    blCheck = false;
                }

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("Unable to close file\n");
                    blCheck = false;
                }
            }
            else
            {
                printf("Unable to Open File\n");
            }
        }
        else
        {
            printf("Null Check Failed\n");
        }

        break;
    } while (1);
    
    
    return blCheck;
}

//**************************.deviceManagerJsonWrite.****************************
// Purpose : Function to Write into a json File
// Inputs  : pstJsonArray - Points to the Object array inside Json file
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerJsonWrite(cJSON **pstJsonArray)
{
    bool blCheck = false;
    FILE *pstFilePointer = NULL;
    int8 *pcJsonString = NULL;
    int8 *pcFileName = "device.json";
    int8 *cMode = "w";

    if (NULL != pstJsonArray)
    {
        do
        {
            if (NULL == *pstJsonArray)
            {
                printf("Unable to parse information\n");
                cJSON_Delete(*pstJsonArray);

                break;
            }

            pcJsonString = cJSON_Print(*pstJsonArray);
                    
            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                if (true == fileOperationWrite(pcJsonString, 
                                &pstFilePointer))
                {
                    cJSON_free(pcJsonString);
                    blCheck = true;
                }
                else
                {
                    printf("Unable to write in file\n");
                }

                cJSON_Delete(*pstJsonArray);

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("Unable to close file\n");
                    blCheck = false;
                }

                break;
            }

        } while (1);
    }
    else
    {
        printf("Null Check Failed\n");
    }

    return blCheck;
}

//************************.deviceManagerJsonPrint.******************************
// Purpose : Function to Add a Device to list
// Inputs  : pstJsonArray - Points to the Object which contain device details
//           pstDeviceDetails - Points to the specific device information 
//           that need to be printed
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerJsonPrint(cJSON **pstJsonObject,
                            cJSON **pstDeviceDetails)
{
    bool blCheck = false;
    int16 nCount = 0;
    const int8 *pcState[MAX_MENU_COUNT] = {"Online", "Charging", 
                                         "Disabled", "Offline"};

    
    if ((NULL != pstJsonObject) && (NULL != pstDeviceDetails))
    {
        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Device ID");

        if (true == cJSON_IsNumber(*pstDeviceDetails))
        {
            printf("Device ID:\t%d\n", (*pstDeviceDetails)->valueint);
            nCount ++;
        }
        

        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Vendor ID");

        if (true == cJSON_IsNumber(*pstDeviceDetails))
        {
            printf("Vendor ID:\t%d\n", (*pstDeviceDetails)->valueint);
            nCount ++;
        }

        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Device Name");

        if ((true == cJSON_IsString(*pstDeviceDetails)) && 
                    (NULL != (*pstDeviceDetails)->valuestring))
        {
            printf("Device Name:\t%s\n", (*pstDeviceDetails)->valuestring);
            nCount ++;
        }

        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Device State");

        if (true == cJSON_IsNumber(*pstDeviceDetails))
        {
            printf("Device State:\t%s\n", 
                        pcState[(*pstDeviceDetails)->valueint]);
            nCount ++;
        }

        if(MAX_MENU_COUNT == nCount)
        {
            blCheck = true;
        }
    }
    else
    {
        printf("Null Check Failed\n");
    }

    return blCheck;
}

//**************************.deviceManagerSelect.*******************************
// Purpose : Function that lets the user select the option to be executed 
// Inputs  : stDevice - Points to the Menu 
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerSelect( MENU *stDevice)
{
    bool blCheck = false;
    DEVICE_MANAGER *pstHead = NULL;
    int16 nChoice = 0;
    int8 cProceed = 'y';
    int16 nIterator = 0;

    printf("\n**************MAIN MENU*************\n");

    while ('y' == cProceed)
    {
        printf("\t\tENTER\n");

        // prints the menu
        for(nIterator = 0; nIterator < MAX_MENU_COUNT; nIterator ++)
        {
            printf("\t%hd  %s\n", nIterator, stDevice[nIterator].Menu);
        }

        printf("*************************************\n\n");
        scanf("%hd", &nChoice);

        if (0 == nChoice)
        {
            pstHead = NULL;
        }

        if((0 <= nChoice) && (MAX_MENU_COUNT > nChoice))
        {
            if(true != stDevice[nChoice].pManager(&pstHead))
            {
                printf("Error Found during execution\n");
                
            }

            if (0 == nChoice)
            {
                deviceManagerFree(&pstHead);
            }
            blCheck = true;
        }
        else
        {
            printf("\tINVALID OPTION\n");
        }

        printf("\n**************MAIN MENU*************\n");
        printf("\tPress 'y' to Continue\n\tPress 'n' to Stop\n");
        printf("*************************************\n\n");
        while(getchar() != '\n');
        scanf("%c", &cProceed);

        if ('y' == cProceed)
        {
            printf("\x1b[H\n\x1b[J");// Move Cursor to top-left and clear screen
            printf("\n**************MAIN MENU*************\n");
        }
        if (('y' != cProceed) && ('n' != cProceed))
        {
            printf("\tINVALID CHOICE\n");
        }
    }    

    return blCheck;
}

//EOF

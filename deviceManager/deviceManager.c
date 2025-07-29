//***************************** Device Manager *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : deviceManager.c
// Summary : Manages different devices. Add, delete, search and display devices 
//           using a json file and linked list
// Note    : None
// Author  : Bimesh Raj K R
// Date    : 23/Jul/2025
//******************************************************************************

//****************************** Include Files *********************************
#include "deviceManager.h"
#include "memoryManager.h"
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
// Inputs  : ppstHead - Pointer to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerAdd(DEVICE_MANAGER **ppstHead)
{
    bool blCheck = false;
    uint16 unChoice = 0;
    uint16 unIterator = 0;
    uint16 unFlag = 0;
    DEVICE_MANAGER *pstCurrentNode = NULL;
    DEVICE_MANAGER *pstNewNode = NULL;
    const int8 *pcState[MAX_STATE_COUNT] = {"Online", "Charging", 
                                         "Disabled", "Offline"};

    while (0 == unChoice)
    {
        pstCurrentNode = *ppstHead;

        if (true == memoryManagerAllocate((void **)&pstNewNode, 
                                           sizeof(DEVICE_MANAGER)))
        {
            printf("\nEnter Device Id: ");

            if (1 != scanf("%ld", &pstNewNode->ulDeviceId))
            {
                printf("\tINVALID DEVICE ID\n");
                unFlag = 1;

                break;
            }

            while ('\n' != getchar());

            printf("\nEnter Vendor Id: ");

            if (1 != scanf("%ld", &pstNewNode->ulVendorId))
            {
                printf("\tINVALID VENDOR ID\n");
                unFlag = 1;

                break;
            }

            while ('\n' != getchar());

            printf("\nEnter Device Name: ");
            scanf("%s", pstNewNode->cName);

            while ('\n' != getchar());

            printf("\n**********DEVICE STATE**********\n");

            for (unIterator = 0; unIterator < MAX_STATE_COUNT; unIterator ++)
            {
                printf("\t%s = %hd\n", pcState[unIterator], unIterator);
            }

            printf("*********************************\n\n");
            printf("\nEnter Device State: ");

            if (1 != scanf("%hd", &pstNewNode->unState))
            {
                printf("\tINVALID DEVICE STATE\n");
                unFlag = 1;

                break;
            }

            while ('\n' != getchar());

            if (MAX_STATE_COUNT <= pstNewNode->unState)
            {
                printf("\n\tDEVICE STATE OUTSIDE RANGE\n");

                if (true != memoryManagerFree((void **)&pstNewNode))
                {
                    printf("\tFailed to release memory\n");
                }

                pstNewNode = NULL;

                break;
            }

            pstNewNode->pstNext = NULL;

            if (NULL == *ppstHead)
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

            if (true != deviceManagerJson(&pstNewNode))
            {
                printf("\tCannot Add Device info to json file\n");

                break;
            }

            blCheck = true;
        }
        else
        {   
            printf("\tMemory Allocation Failed\n");

            break;
        }

        printf("\n***************ADD MENU*************\n");
        printf("\tPress 0 to Continue\n\tPress 1 to Stop\n");
        printf("*************************************\n\n");

        if (1 != scanf("%hd", &unChoice))
        {
            printf("\tINVALID CHOICE\n");

            while ('\n' != getchar());

            break;
        }

        if ((0 != unChoice) && (1 != unChoice))
        {
            printf("\tCHOICE OUT OF RANGE\n");
            
            while ('\n' != getchar());
        }
    }

    if (1 == unFlag)
    {
        if (true != memoryManagerFree((void **)&pstNewNode))
        {
            printf("\tFailed to release memory\n");
        }
    }

    return blCheck;
}

//***************************.deviceManagerJson.********************************
// Purpose : Function to Add a Device Details to the json File
// Inputs  : pstCurrentNode - Pointer to the node currently made in the 
//           linked list
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerJson(DEVICE_MANAGER **pstCurrentNode)
{
    bool blCheck = false;
    uint32 ulFileSize = 0;
    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    FILE *pstFilePointer = NULL;
    int8 *pcJsonString = NULL;
    int8 *pcFileData = NULL;
    int8 *pcFileName = FILE_NAME;
    int8 *cMode = READ_MODE;

    do
    {
        if (NULL != pstCurrentNode)
        {
            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                if (true != fileOperationGetSize(&pstFilePointer, 0, 
                                        SEEK_END, &ulFileSize))
                {
                    printf("\tFile size could not be determined\n");

                    break;
                }

                if (true == memoryManagerAllocate((void **)&pcFileData, 
                                                   ulFileSize + 1))
                {
                    if (true != fileOperationRead(pcFileData, 1, 
                                ulFileSize, &pstFilePointer))
                    {
                        printf("\tFile Cannot be Read\n");
                        free(pcFileData);

                        break;
                    }

                    pcFileData[ulFileSize] = NULL_CHARACTER;
                    pstJsonArray = cJSON_Parse(pcFileData);

                    if (true != memoryManagerFree((void **)&pcFileData))
                    {
                        printf("\tFailed to release memory\n");

                        break;
                    }
                }
                else
                {
                    printf("\tMemory Allocation Failed");
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
                    printf("\tUnable to close file\n");

                    break;
                }
            }
            else
            {
                pstJsonArray = cJSON_CreateArray();
            }

            pstFilePointer = NULL;
            cMode = WRITE_MODE;

            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                pstJsonObject = cJSON_CreateObject();
                cJSON_AddNumberToObject(pstJsonObject, "Device ID", 
                    (*pstCurrentNode)->ulDeviceId);
                cJSON_AddNumberToObject(pstJsonObject, "Vendor ID", 
                    (*pstCurrentNode)->ulVendorId);
                cJSON_AddStringToObject(pstJsonObject, "Device Name", 
                    (*pstCurrentNode)->cName);
                cJSON_AddNumberToObject(pstJsonObject, "Device State", 
                    (*pstCurrentNode)->unState);
                cJSON_AddItemToArray(pstJsonArray, pstJsonObject);

                pcJsonString = cJSON_Print(pstJsonArray);
                cJSON_Delete(pstJsonArray);

                if (true != fileOperationWrite(pcJsonString, 
                                &pstFilePointer))
                {
                    printf("\tUnable to write in file\n");

                    break;
                }

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("\tUnable to close file\n");

                    break;
                }

                blCheck = true;

                break;
            }
            else
            {
                printf("\tUnable to open the file.\n");
                break;
            }
        }
        else
        {
            printf("\tNull Check failed\n");
            break;
        }

    }while(1);

    return blCheck;
}

//*************************.deviceManagerDelete.********************************
// Purpose : Function to Delete a Device from list
// Inputs  : ppstHead - Pointer to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerDelete(DEVICE_MANAGER **ppstHead)
{
    uint32 ulSearchDevice = 0;
    bool blCheck = false;
    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    cJSON *pstDeviceDetails = NULL;
    uint32 ulIterator = 0;

    if (NULL != ppstHead)
    {
        do
        {
            printf("Enter The Device ID to Remove: ");

            if (1 != scanf("%ld", &ulSearchDevice))
            {
                printf("\tINVALID DEVICE ID\n");

                break;
            }

            while ('\n' != getchar());

            if (true != deviceManagerJsonRead(&pstJsonArray))
            {
                printf("\tFile doesn't exist\n");

                break;
            }

            if (true == cJSON_IsArray(pstJsonArray))
            {
                for (ulIterator = 0; 
                        ulIterator < (uint32)cJSON_GetArraySize(pstJsonArray);
                        ulIterator ++)
                {
                    pstJsonObject = cJSON_GetArrayItem(pstJsonArray, 
                                                       ulIterator);
                    pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            pstJsonObject, "Device ID");

                    if((true == cJSON_IsNumber(pstDeviceDetails)) && 
                        ((uint32)pstDeviceDetails->valueint == ulSearchDevice))
                    {
                        cJSON_DeleteItemFromArray(pstJsonArray, ulIterator);
                        printf("\n\tDEVICE REMOVED\n");
                        blCheck = true;
                    }
                }

                if ((ulIterator == (uint32)cJSON_GetArraySize(pstJsonArray)) && 
                                        (true != blCheck))
                {
                    printf("\n\tDEVICE NOT FOUND\n");
                    blCheck = true;
                }

                if (true != deviceManagerJsonWrite(&pstJsonArray))
                {
                    printf("\tUnable to write in the file\n");

                    blCheck = false;

                    break;
                }

                break;
            }

            printf("\tInvalid file format\n");

            break;

        }while (1);
    }
    else
    {
        printf("\tNull Check failed\n");
    }

    return blCheck;
}

//**************************.deviceManagerSearch.*******************************
// Purpose : Function to Search a Device from the list
// Inputs  : ppstHead - Pointer to the head node
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerSearch(DEVICE_MANAGER **ppstHead)
{
    uint32 ulSearchDevice = 0;
    bool blCheck = false;
    cJSON *pstJsonObject = NULL;
    cJSON *pstJsonArray = NULL;
    cJSON *pstDeviceDetails = NULL;
    uint32 ulIterator = 0;

    if (NULL != ppstHead)
    {
        do
        {
            printf("Enter The Device ID to search: ");

            if (1 != scanf("%ld", &ulSearchDevice))
            {
                printf("\tINVALID DEVICE ID\n");

                break;
            }

            while ('\n' != getchar());

            if (true != deviceManagerJsonRead(&pstJsonArray))
            {
                printf("\tFile doesn't exist\n");

                break;
            }

            if (true == cJSON_IsArray(pstJsonArray))
            {
                for (ulIterator = 0; 
                        ulIterator < (uint32)cJSON_GetArraySize(pstJsonArray);
                        ulIterator ++)
                {
                    pstJsonObject = cJSON_GetArrayItem(pstJsonArray, 
                                                       ulIterator);
                    pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            pstJsonObject, "Device ID");

                    if((true == cJSON_IsNumber(pstDeviceDetails)) && 
                        ((uint32)pstDeviceDetails->valueint == ulSearchDevice))
                    {
                        printf("\tDEVICE FOUND\n");

                        if (true == deviceManagerJsonPrint(&pstJsonObject, 
                                                        &pstDeviceDetails))
                        {
                            blCheck = true;
                        }
                        else
                        {
                            printf("\tThe Device Details could not be"
                                   "printed\n");
                        }

                        break;
                    }
                }

                if ((ulIterator == (uint32)cJSON_GetArraySize(pstJsonArray)) && 
                            (true != blCheck))
                {
                    cJSON_Delete(pstJsonArray);

                    printf("\n\tDEVICE NOT FOUND\n");
                    blCheck = true;

                    break;
                }

                cJSON_Delete(pstJsonArray);

                break;
            }

            printf("\tInvalid file format\n");

            break;

        }while (1);
    }
    else
    {
        printf("\tNull Check failed\n");
    }

    return blCheck;
}

//**************************.deviceManagerDisplay.******************************
// Purpose : Function to Display all Devices
// Inputs  : ppstHead - Pointer to the head node
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
    uint32 ulIterator = 0;

    if (NULL != ppstHead)
    {
        do
        {
            if (true != deviceManagerJsonRead(&pstJsonArray))
            {
                printf("\tFile doesn't exist\n");

                break;
            }

            if (true == cJSON_IsArray(pstJsonArray))
            {
                for (ulIterator = 0; 
                    ulIterator < (uint32)cJSON_GetArraySize(pstJsonArray);
                    ulIterator ++)
                {
                    pstJsonObject = cJSON_GetArrayItem(pstJsonArray, 
                                                       ulIterator);

                    printf("\n\tDEVICE: %ld\n", ulIterator + 1);
                    
                    if (true == deviceManagerJsonPrint(&pstJsonObject, 
                                                        &pstDeviceDetails))
                    {
                        blCheck = true;
                    }
                    else
                    {
                        printf("\tThe Device Details could not be printed\n");

                        break;
                    }
                }

                cJSON_Delete(pstJsonArray);

            }
            else
            {
                printf("\tInvalid file format\n");;
            }

            break;

        }while (1);
    }
    else
    {
        printf("\tNull Check failed\n");
    }
    return blCheck;
}

//**************************.deviceManagerFree.*********************************
// Purpose : Function that frees up memory
// Inputs  : ppstHead - Pointer to the head node
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
    uint16 unFlag = 0;

    if ((NULL != ppstHead) && (NULL != *ppstHead))
    {
        pstCurrentNode = *ppstHead;

        while (NULL != pstCurrentNode) 
        {
            pstNext = pstCurrentNode->pstNext;

            if (true != memoryManagerFree((void **)&pstCurrentNode))
            {
                printf("\tFailed to release memory\n");
                unFlag = 1;

                break;
            }

            pstCurrentNode = pstNext;
        }

        if (0 == unFlag)
        {
            blCheck = true;
        }
    }
    

    return blCheck;
}

//**************************.deviceManagerJsonRead.*****************************
// Purpose : Function to Read a Json file
// Inputs  : pstJsonArray - Pointer to the Object array inside Json file
// Outputs : None
// Return  : true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
static bool deviceManagerJsonRead(cJSON **pstJsonArray)
{
    bool blCheck = false;
    uint32 ulFileSize = 0;
    int8 *pcFileData = NULL;
    FILE *pstFilePointer = NULL;
    int8 *pcFileName = FILE_NAME;
    int8 *cMode = READ_MODE;
    uint32 ulOffset = 0;
    uint32 ulPosition = 0;

    do
    {
        if (NULL != pstJsonArray)
        {
            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                if (NULL == pstFilePointer)
                {
                    printf("\tNull Check Failed\n");

                    break;
                }

                ulPosition = SEEK_END;

                if (true != fileOperationGetSize(&pstFilePointer, ulOffset, 
                                            ulPosition, &ulFileSize))
                {
                    printf("\tFile size could not be determined\n");

                    break;
                }

                if (true == memoryManagerAllocate((void **)&pcFileData, 
                                                   ulFileSize + 1))
                {
                    if (true != fileOperationRead(pcFileData, 1, 
                                ulFileSize, &pstFilePointer))
                    {
                        printf("\tFile Cannot be Read\n");

                        break;
                    }
                    pcFileData[ulFileSize] = '\0';
                    *pstJsonArray = cJSON_Parse(pcFileData);

                    if (true != memoryManagerFree((void **)&pcFileData))
                    {
                        printf("\tFailed to release memory\n");

                        break;
                    }

                    blCheck = true;
                    
                }
                else
                {
                    printf("\tMemory Allocation Failed\n");
                }

                if (NULL == *pstJsonArray)
                {
                    printf("\tUnable to parse information\n");
                    cJSON_Delete(*pstJsonArray);
                    blCheck = false;
                }

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("\tUnable to close file\n");
                    blCheck = false;
                }
            }
            else
            {
                printf("\tUnable to Open File\n");
            }
        }
        else
        {
            printf("\tNull Check Failed\n");
        }

        break;

    } while (1);
    
    
    return blCheck;
}

//**************************.deviceManagerJsonWrite.****************************
// Purpose : Function to Write into a json File
// Inputs  : pstJsonArray - Pointer to the Object array inside Json file
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
    int8 *pcFileName = FILE_NAME;
    int8 *cMode = WRITE_MODE;

    if (NULL != pstJsonArray)
    {
        do
        {
            if (NULL == *pstJsonArray)
            {
                printf("\tUnable to parse information\n");
                cJSON_Delete(*pstJsonArray);

                break;
            }

            pcJsonString = cJSON_Print(*pstJsonArray);
                    
            if (true == fileOperationOpen(&pstFilePointer, pcFileName, cMode))
            {
                if (true == fileOperationWrite(pcJsonString, 
                                &pstFilePointer))
                {
                    blCheck = true;
                }
                else
                {
                    printf("\tUnable to write in file\n");
                }

                cJSON_Delete(*pstJsonArray);

                if (true != fileOperationClose(&pstFilePointer))
                {
                    printf("\tUnable to close file\n");
                    blCheck = false;
                }

                break;
            }

        } while (1);
    }
    else
    {
        printf("\tNull Check Failed\n");
    }

    return blCheck;
}

//************************.deviceManagerJsonPrint.******************************
// Purpose : Function to Add a Device to list
// Inputs  : pstJsonArray - Pointer to the Object which contain device details
//           pstDeviceDetails - Pointer to the specific device information 
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
    uint16 unCount = 0;
    const int8 *pcState[MAX_MENU_COUNT] = {"Online", "Charging", 
                                         "Disabled", "Offline"};

    
    if ((NULL != pstJsonObject) && (NULL != pstDeviceDetails))
    {
        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Device ID");

        if (true == cJSON_IsNumber(*pstDeviceDetails))
        {
            printf("Device ID:\t%d\n", (*pstDeviceDetails)->valueint);
            unCount ++;
        }
        

        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Vendor ID");

        if (true == cJSON_IsNumber(*pstDeviceDetails))
        {
            printf("Vendor ID:\t%d\n", (*pstDeviceDetails)->valueint);
            unCount ++;
        }

        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Device Name");

        if ((true == cJSON_IsString(*pstDeviceDetails)) && 
                    (NULL != (*pstDeviceDetails)->valuestring))
        {
            printf("Device Name:\t%s\n", (*pstDeviceDetails)->valuestring);
            unCount ++;
        }

        *pstDeviceDetails = cJSON_GetObjectItemCaseSensitive(
                            *pstJsonObject, "Device State");

        if (true == cJSON_IsNumber(*pstDeviceDetails))
        {
            printf("Device State:\t%s\n", 
                        pcState[(*pstDeviceDetails)->valueint]);
            unCount ++;
        }

        if(MAX_MENU_COUNT - 1 == unCount)
        {
            blCheck = true;
        }
    }
    else
    {
        printf("\tNull Check Failed\n");
    }

    return blCheck;
}

//**************************.deviceManagerSelect.*******************************
// Purpose : Function that lets the user select which option from the menu to 
//           execute (add, delete, search,display)
// Inputs  : stDevice - Pointer to the Menu 
// Outputs : None
// Return  : blCheck - true if there are no errors and false if any errors exist
//           during function execution
// Notes   : None
//******************************************************************************
bool deviceManagerSelect( DEVICE_MENU *stDevice)
{
    bool blCheck = false;
    DEVICE_MANAGER *pstHead = NULL;
    uint16 unChoice = 0;
    uint16 unIterator = 0;

    while (1)
    {
        printf("\n**************MAIN MENU*************\n");
        printf("\t\tENTER\n");

        if (NULL == stDevice)
        {
            printf("Menu could not be displayed\n");

            break;
        }

        // Prints the menu
        for (unIterator = 0; unIterator < MAX_MENU_COUNT; unIterator ++)
        {
            printf("\t%hd  %s\n", unIterator, stDevice[unIterator].Menu);
        }

        printf("*************************************\n\n");

        if(1 != scanf("%hd", &unChoice))
        {
            printf("\tINVALID CHOICE\n");

            break;
        }

        while('\n' != getchar());

        if (EXIT_CHOICE == unChoice)//To exit Menu
        {
            break;
        }

        if (0 == unChoice)// For Making the Linked list
        {
            pstHead = NULL;
        }

        if(MAX_MENU_COUNT > unChoice)
        {
            if(true != stDevice[unChoice].pManager(&pstHead))
            {
                printf("\tError Found during execution\n");

                break; 
            }

            if (0 == unChoice)
            {
                deviceManagerFree(&pstHead);
            }
            blCheck = true;
        }
        else
        {
            printf("\tINVALID OPTION\n");
        }
    }    

    return blCheck;
}

//EOF

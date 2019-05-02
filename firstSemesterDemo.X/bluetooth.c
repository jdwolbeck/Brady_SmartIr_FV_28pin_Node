#include "bluetooth.h"
#include "app.h"
#include "uart.h"
#include "gpio.h"
#include <string.h>
#include <stdbool.h>
#include <p24FV16KM202.h>

COMMAND_BYTE command_byte;
BLE_DATA bleData;

char MAC_LAST[20] = "801F12BC4606";
char MAC_THIS[20] = "801F12B58D2F";
char MAC_NEXT[20] = "801F12BC47B6";

void BLE_init(void)
{
    //Used for my debug LED
    ANSBbits.ANSB6 = 0;
    TRISBbits.TRISB6 = 0;
    //
    int i;
    for(i = 0; i < READINGS; i++)
    {
        memset(bleData.data[i],'\0',DATA_LEN);
    }
    memset(bleData.packetBuf,'\0',PACKET_LEN);
    bleData.dataReceived = false;
    bleData.isConnected = false;
    bleData.isTryingConn = false;
    bleData.packetIndex = 0;
    bleData.searchStreamEn = true;
    bleData.searchCmdEn = true;
    bleData.searchMacEn = true;
    
    command_byte = IDLE;
}

void BLE_reboot(void)
{
    memset(bleData.packetBuf,'\0',PACKET_LEN);
    bleData.packetIndex = 0;
    bleData.isConnected = false;
    bleData.isTryingConn = false;
    bleData.searchStreamEn = true;
    bleData.searchCmdEn = true;
    bleData.searchMacEn = true;
}

void node_application(void)
{
    if(bleData.dataReceived)
    {
        if(BLE_searchStr("SEND_DATA,0", bleData.packetBuf))
        {//Enter when previous nodes wants this nodes data
            command_byte = SEND_DATA;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            LED_JOSH = 1;
        }
        if(BLE_searchStr("SEND_DATA,1", bleData.packetBuf))
        {//Enter when previous nodes wants next nodes data
            command_byte = CONNECT_NEXT;
            BLE_disconnect();
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
        }        
        if(BLE_searchStr("SEND_DATA,2", bleData.packetBuf))
        {//Enter when previous nodes wants next nodes next data
            command_byte = SEND_MAC;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
        }        
        if(BLE_searchStr("REBOOT", bleData.packetBuf))
        {//Reset all variables if bluetooth transmits REBOOT
            BLE_reboot();
            command_byte = IDLE;
        }        
    }
    
    if(command_byte == IDLE)
    {
        
    }
    else if(command_byte == SEND_DATA)
    {
        getConversions();
        command_byte = IDLE;
    } 
    else if(command_byte == CONNECT_NEXT)
    {
        bleData.data[0][0] = '1';
        bleData.data[0][1] = '2';
        bleData.data[0][2] = '3';
        bleData.data[0][3] = '\0';
        bleData.data[1][0] = '4';
        bleData.data[1][1] = '5';
        bleData.data[1][2] = '6';
        bleData.data[1][3] = '\0';
        bleData.data[2][0] = '7';
        bleData.data[2][1] = '8';
        bleData.data[2][2] = '9';
        bleData.data[2][3] = '\0';
        command_byte = CONNECT_LAST;
    }
//    else if(command_byte == CONNECT_NEXT)
//    {
//        if(BLE_searchStr("CMD>", bleData.packetBuf))
//        {//If CMD mode successfully entered
//            memset(bleData.packetBuf,'\0',PACKET_LEN);
//            bleData.packetIndex = 0;
//            BLE_connect(2,1);
//        }
//        if(BLE_searchStr(MAC_NEXT, bleData.packetBuf))
//        {//Search for the MAC address of the next node for this node
//            memset(bleData.packetBuf,'\0',PACKET_LEN);
//            bleData.packetIndex = 0;
//            BLE_connect(3,1);
//        }
//        if(BLE_searchStr("STREAM_OPEN", bleData.packetBuf) && bleData.isTryingConn)
//        {//Once stream is open, tell next node to send back ITS data
//            memset(bleData.packetBuf,'\0',PACKET_LEN);
//            bleData.packetIndex = 0;
//            bleData.isConnected = true;
//            uart_print("SEND_DATA,0");
//        }
//        if(bleData.isConnected)
//        {
//            if(BLE_parseData(bleData.packetBuf))
//            {
//                uart_print("K,1\r");
//                ms_delay(50);
//                bluetooth_reboot();
//                command_byte = CONNECT_LAST;
//            }
//        }
//        if(!bleData.isTryingConn)
//        {//If this is the first time in this if, do this
//            bluetooth_reboot();
//            ms_delay(250);
//            BLE_connect(1, 1);
//        }
//    }
    else if(command_byte == CONNECT_LAST)
    {
        if(bleData.searchCmdEn && BLE_searchStr("CMD>", bleData.packetBuf))
        {//If CMD mode successfully entered
            bleData.searchCmdEn = false;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            BLE_connect(2,0);
        }
        if(bleData.searchMacEn && BLE_searchStr(MAC_LAST, bleData.packetBuf))
        {//Search for the MAC address of the next node for this node
            bleData.searchMacEn = false;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            BLE_connect(3,0);
        }
        if(bleData.searchStreamEn && BLE_searchStr("STREAM_OPEN", bleData.packetBuf))
        {
            bleData.searchStreamEn = false;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            bleData.isConnected = true;
        }
        if(bleData.isConnected && BLE_searchStr("AOK", bleData.packetBuf))
        {
//            int i = 0;
//            while(bleData.data[i][0] != '\0')
//            {
//                uart_print(bleData.data[i]);
//                uart_print(",");
//            }
            uart_print("123,456,789,");
            BLE_reboot();
            command_byte = IDLE;            
        }
        if(!bleData.isTryingConn)
        {//If this is the first time in this if, do this
            bleData.isTryingConn = true;
            BLE_connect(1, 0);
        }
    }
    else if(command_byte == SEND_MAC)
    {
        
    }
    else if(command_byte == DISCONNECT)
    {
        
    }
    else if(command_byte == TEST)
    {
        if(bleData.searchCmdEn && BLE_searchStr("CMD>", bleData.packetBuf))
        {//If CMD mode successfully entered
            bleData.searchCmdEn = false;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            BLE_connect(2,0);
        }
        if(bleData.searchMacEn && BLE_searchStr(MAC_LAST, bleData.packetBuf))
        {//Search for the MAC address of the next node for this node
            bleData.searchMacEn = false;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            BLE_connect(3,0);
        }
        if(bleData.searchStreamEn && BLE_searchStr("STREAM_OPEN", bleData.packetBuf))
        {
            bleData.searchStreamEn = false;
            memset(bleData.packetBuf,'\0',PACKET_LEN);
            bleData.packetIndex = 0;
            bleData.isConnected = true;
        }
        if(bleData.isConnected)
        {
            uart_print("123,456,789,");
            command_byte = DISCONNECT;
        }
        if(BLE_searchStr("DISCONNECT", bleData.packetBuf))
        {
            uart_print("R,1");
            BLE_reboot();
        }
        if(BLE_searchStr("REBOOT", bleData.packetBuf))
        {
            BLE_reboot();
        }
        if(!bleData.isTryingConn)
        {//If this is the first time in this state, do this
            bleData.isTryingConn = true;
            BLE_connect(1,0);
        }
    }
}

void BLE_connect(int count, int dir)
{
    //This function is used to actually connect to the RN4871
    //(Bluetooth Module). Count is used form rest of program
    //to control what commands are being sent.
    
    if(count == 1)
    {
        uart_print("$");
        ms_delay(220);
        uart_print("$$");  //CMD mode
    }
    else if(count == 2)
    {
        uart_print("F\r"); //Search mode
    }
    else if(count == 3 && dir == 1)
    {
        uart_print("C,0,"); //Connect to module
        uart_print(MAC_NEXT);
        uart_print("\r");
    }
    else if(count == 3 && dir == 0)
    {
        uart_print("C,0,"); //Connect to module
        uart_print(MAC_LAST);
        uart_print("\r");
    }
}

bool BLE_searchStr(char key[], char str[])
{
    //This function will parse an input string (str) and
    //find the keyword within it. Returns true if found.
    
    char temp[strlen(key)];
    int i = strlen(key) - 1, j, k = 0;
    
    while(str[i++] != '\0')
    {
        for(j = 0; j < strlen(key); j++)
        {
            temp[j] = str[j+k];
        }
        temp[j] = '\0';
        if(!strcmp(temp,key))
            return true;
        k++;
    }
    return false;
}

bool BLE_parseData(char str[])
{
    //This function is used after bluetooth connection is
    //established. This is used to determine when all three
    //data points have been received. Then takes these values
    //and puts them into their respective buffer. 
    //bleData.data[0-2] holds soil, lux, temp data.
    
    int i,j,k,n;
    int count=0;
    
    for(i = 0; bleData.packetBuf[i] != '\0'; i++)
    {
        if(bleData.packetBuf[i] == ',')
            count++;
    }
    
    if(count < 3)
        return false;
//    j/k : are the buffer index
//    i : represents a particular data
//    n : is the specific data's value index
    k = 0;
    for(i = 0; i < 3; i++)
    {
        n = 0;
        for(j = k; bleData.packetBuf[j] != ','; j++)
        {
            bleData.data[i][n++] = bleData.packetBuf[j];
            k++;
        }
        k++;
    }
    return true;
}

void BLE_disconnect()
{
    BLE_connect(1,0);
    while(!BLE_searchStr("CMD>", bleData.packetBuf));
    uart_print("K,1\r");
    while(!BLE_searchStr("DISCONNECT", bleData.packetBuf));
    uart_print("---\r");
    memset(bleData.packetBuf,'\0',PACKET_LEN);
    bleData.packetIndex = 0;
    uart_print("\r\n--Disconnect--\r\n");
    BLE_reboot();
}

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DATA_LEN 5
#define READINGS 3
#define STR_LEN 500
#define PACKET_LEN 1024

#define LED_JOSH LATBbits.LATB6

extern char MAC_LAST[20];
extern char MAC_THIS[20];
extern char MAC_NEXT[20];

typedef enum{
    IDLE = 0x00,
    CONNECT_NEXT = 0x01,
    CONNECT_LAST = 0x02,
    SEND_MAC = 0x03,
    DISCONNECT = 0x04,
    SEND_DATA = 0x05,
    TEST = 0x0F
} COMMAND_BYTE;

typedef struct
{
    int packetIndex;
    char packetBuf[PACKET_LEN];
    char data[READINGS][DATA_LEN];
    bool isConnected;
    bool dataReceived;
    bool isTryingConn;
    bool searchCmdEn;
    bool searchMacEn;
    bool searchStreamEn;
} BLE_DATA;

extern BLE_DATA bleData;

void BLE_init(void);
void BLE_reboot(void);
void node_application(void);
void BLE_connect(int,int);
bool BLE_searchStr(char[],char[]);
bool BLE_parseData(char[]);
void BLE_disconnect(void);
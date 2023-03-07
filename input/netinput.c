#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <input_manager.h>

#define SERVER_PORT 8888
static int g_iSocketServer;

static int NetinputGetInputEvent(PInputEvent ptInputEvent)
{
    struct sockaddr_in tSocketClientAddr;
    int iRecvLen;
    char aRecvBuf[1000];
    unsigned int iAddrLen = sizeof(struct sockaddr_in);

    iRecvLen = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
    if(iRecvLen > 0)
    {
        aRecvBuf[iRecvLen] = '\0';
        ptInputEvent->iType = INPUT_TYPE_NET;
        gettimeofday(&ptInputEvent->tTime, NULL);
        strncpy(ptInputEvent->str, aRecvBuf, 1000);
        ptInputEvent->str[999] = '\0';
        return 0;
    }
    
    return -1;
}

static int NetinputDeviceInit(void)
{
    struct sockaddr_in tSocketServerAddr;
    int iRet;
    /*建立socket*/
    g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if(g_iSocketServer < 0)
    {
        printf("socket err\n");
        return -1;
    }
    /*配置接收端服务器*/
    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT);
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if(iRet < 0)
    {
        printf("bind error\n");
        return -1;
    }

    return 0;
}

static int NetinputDeviceExit(void)
{
    close(g_iSocketServer);
    return 0;
}


static InputDevice g_tNetinputDev = {
    .name = "touchscreen",
    .GetInputEvent = NetinputGetInputEvent,
    .DeviceInit = NetinputDeviceInit,
    .DeviceExit = NetinputDeviceExit,
};

#if 1
int main(int argc, char **argv)
{
    InputEvent event;
    int ret;

    g_tNetinputDev.DeviceInit();

    while(1)
    {
        ret = g_tNetinputDev.GetInputEvent(&event);
        if(ret)
        {
            printf("GetInputEvent err\n");
            return -1;
        }
        else{
            printf("Type       : %d\n", event.iType);
            printf("st         : %s\n", event.str);
        }
    }
}
#endif
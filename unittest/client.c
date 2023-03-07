#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;

    int iRet;
    int iSendLen;
    int iAddrLen;

    if(argc != 3)
    {
		printf("Usage:\n");
		printf("%s <server_ip> <str>\n", argv[0]);
		return -1;        
    }

    iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT);
    if(inet_aton(argv[1], &tSocketServerAddr.sin_addr) == 0)
    {
 		printf("invalid server_ip\n");
		return -1;       
    }
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iAddrLen = sizeof(struct sockaddr_in);
    iSendLen = sendto(iSocketClient, argv[2], strlen(argv[2]), 0, (const struct sockaddr *)&tSocketServerAddr, iAddrLen);
    close(iSocketClient);

    return 0;
}
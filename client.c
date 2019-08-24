#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include"info.h"
#include"hashtable.h"
#include<string.h>
#include"opserver.h"
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<sys/poll.h>
#include"server.h"

int main(int argc,char* argv[]){

    int clientskt;
    char buffer[256];
    CHECKSOCKET(clientskt=socket(AF_UNIX,SOCK_STREAM,0));
    struct sockaddr_un socket_address;
    memset(&socket_address, 0, sizeof(struct sockaddr_un));     
    socket_address.sun_family = AF_UNIX;
    connect(clientskt,(const struct sockaddr*)&socket_address, sizeof(socket_address));
    printf("messaggio da inviare\n");
    fgets(buffer,255,stdin);
    printf("messaggio ricevuto\n");
    return 0;
}
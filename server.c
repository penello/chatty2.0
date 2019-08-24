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
#include"threadworker.h"


#ifndef _server_c
#define _server_c


int dispatcher(int socket_sv){

    pthread_t thd;
    int n_client[BACKLOG];
    int i=0;

    while(i<BACKLOG){
        CHECKACCEPT(n_client[i]=accept(socket_sv,NULL,NULL));
        CHECKTHREAD(pthread_create(&(thd),NULL,&worker,n_client+i));
        i++;
    }
    return 0;

}






int main(int argc,char* argv[]){
   
    fprintf(stdout,"inizializzo server\n");

    stats=create_dataserver();

    struct sockaddr_un sock;
    sock.sun_family=AF_UNIX;
    strncpy(sock.sun_path,SOCKNAME,108);

    int socket_sv=0;
    CHECKSOCKET(socket_sv=socket(AF_UNIX,SOCK_STREAM,0));
    CHECKBIND(bind(socket_sv,(struct sockaddr*)&sock,sizeof(sock)));
    CHECKLISTEN(listen(socket_sv,BACKLOG));

    dispatcher(socket_sv);

    return 0;
}

#endif

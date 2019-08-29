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




int dispatcher(int socket_sv,connections* stats){

    pthread_t thd;
    int i=0;
    fprintf(stdout,"aspetto connessioni\n");
    while(i<BACKLOG){
        CHECKACCEPT((stats->fd)=accept(socket_sv,NULL,NULL));
        fprintf(stdout,"mi sono connesso con un client\n");
        printf("%d\n",stats->fd);
        CHECKTHREAD(pthread_create(&thd,NULL,&worker,stats));
        fprintf(stdout,"ho fatto il thread per la comunicazione\n");
        i++;
    }
    return 0;

}







int main(int argc,char* argv[]){
   
    fprintf(stdout,"inizializzo server\n");

    connections* stats;
    CHECKMALLOC(stats=malloc(sizeof(connections)));
    memset(stats,0,sizeof(connections));
    stats->sv_stats=create_dataserver();
    fprintf(stdout,"creato data server\n");


    struct sockaddr_un sock;
    sock.sun_family=AF_UNIX;
    strncpy(sock.sun_path,SOCKNAME,108);

    fprintf(stdout,"provo la socket\n");


    int socket_sv=0;
    CHECKSOCKET(socket_sv=socket(AF_UNIX,SOCK_STREAM,0));
    CHECKBIND(bind(socket_sv,(struct sockaddr*)&sock,sizeof(sock)));
    CHECKLISTEN(listen(socket_sv,BACKLOG));

    fprintf(stdout,"sono in listen\n");


    dispatcher(socket_sv,stats);

    free_data_server(stats->sv_stats);

    return 0;
}

#endif

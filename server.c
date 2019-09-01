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
#include<sys/stat.h>
#include<sys/un.h>
#include<sys/poll.h>
#include"server.h"
#include"threadworker.h"
#include<signal.h>
#include<unistd.h>



#ifndef _server_c
#define _server_c

//dichiarazione della struct presente nell'omonimo file.h
data_server* data;
//variabile per la gestione dei segnali
volatile sig_atomic_t Sig;

//handler dei segnali
void sighandler(int sig){
    Sig=sig;
}

//procedura che stampa le statistiche del server
void stampastat(){
    CHECKLOCK1(pthread_mutex_lock(&(data->lock)));
    printf("Size totale degli oggetti dello store:%u\n",(data->size_objstore));
    printf("Numero client connessi:%u\n",(data->n_client_objstore));
    printf("Numero oggetti nello store:%u\n",(data->n_obj_objstore));
    CHECKLOCK1(pthread_mutex_unlock(&(data->lock)));
}

//procedura che inizializza correttamente gli handler dei segnali
void sethandler(){
    struct sigaction sigterm;
    struct sigaction sigint;
    struct sigaction sigusr1;
    struct sigaction sigpipe;

    memset(&sigterm,0,sizeof(struct sigaction));
    memset(&sigint,0,sizeof(struct sigaction));
    memset(&sigusr1,0,sizeof(struct sigaction));
    memset(&sigpipe,0,sizeof(struct sigaction));

    sigpipe.sa_handler=SIG_IGN;
    sigterm.sa_handler=sighandler;
    sigint.sa_handler=sighandler;
    sigusr1.sa_handler=sighandler;


    SIG_ACTION(SIGPIPE,sigpipe);
    SIG_ACTION(SIGINT,sigint);
    SIG_ACTION(SIGUSR1,sigusr1);
    SIG_ACTION(SIGTERM,sigterm);
}

//funzione che cicla e per ogni richiesta di connessione crea un thread associato a un solo client
int dispatcher(int socket_sv,pthread_t* thd){
    
    int i=0;
    int k=0;
    while(i<BACKLOG){
        connections* stats;
        if((k=accept(socket_sv,NULL,NULL))==-1){
            fprintf(stderr,"Error in accept connection\n");
        }
        else {
            CHECKMALLOC(stats=malloc(sizeof(connections)));
            memset(stats,0,sizeof(connections));
            stats->fd=k;
            stats->sv_stats=data;
            CHECKTHREAD(pthread_create(&thd[i],NULL,&worker,stats),k);
            i++;
        }
        if(Sig==SIGINT){
            fprintf(stderr,"Server received a SIGINT\n");
            return i;
        }
        else if(Sig==SIGTERM){
            fprintf(stderr,"Server received a SIGTERM\n");
            return i;
        }
        else if(Sig==SIGUSR1){
            fprintf(stderr,"Server received a SIGUSR1\n");
            stampastat();
            Sig=0;
        }
    }
    return i;
}


//main del server dove viene inizializzato e si mette in listen
int main(int argc,char* argv[]){
   

    fprintf(stdout,"inizializzo server\n");

    sethandler();

    data=create_dataserver();
    pthread_t* thd;
    CHECKMALLOC(thd=malloc(BACKLOG*sizeof(pthread_t)));
    memset(thd,0,BACKLOG*sizeof(pthread_t));
    struct sockaddr_un sock;
    sock.sun_family=AF_UNIX;
    strncpy(sock.sun_path,SOCKNAME,108);


    int socket_sv=0;
    CHECKSOCKET(socket_sv=socket(AF_UNIX,SOCK_STREAM,0));
    CHECKBIND(bind(socket_sv,(struct sockaddr*)&sock,sizeof(sock)));
    CHECKLISTEN(listen(socket_sv,BACKLOG));

    int q=mkdir("data",0777);
    if(q<0){
        exit(EXIT_FAILURE);
    }
    chdir("data");

    int n=dispatcher(socket_sv,thd);
    for(int i=0;i<n;i++){
        pthread_join(thd[i],NULL);
    }
    free(thd);
    free_data_server(data);

    return 0;
}

#endif

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
#include<unistd.h>

#ifndef _gestionesig_c
#define _gestionesig_c

void gestionesegnali(int sig){
    if(sig==2){
        fprintf(stderr, "OBJSTORE: Received SIGINT\n");
        exit(EXIT_SUCCESS);
    }
    else if(sig==15){
        fprintf(stderr, "OBJSTORE: Received SIGTERM\n");
        exit(EXIT_SUCCESS);
    }
    else if(sig==16){
        
    }
}

#endif
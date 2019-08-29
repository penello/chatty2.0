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


#ifndef _server_h
#define _server_h

typedef struct{
    data_server* sv_stats;
    int fd;
}connections;


int dispatcher(int socket_sv,connections* stats);

#endif
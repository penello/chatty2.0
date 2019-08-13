#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include"info.h"
#include"hashtable.h"
#include<string.h>
#ifndef _opserver_h_
#define _opserver_h_

typedef struct{
    node** client_connessi;
    pthread_mutex_t* hashlocks;
    unsigned int size_objstore;
    unsigned int n_obj_objstore;
    unsigned int n_client_objstore;
    pthread_mutex_t lock;
} data_server;

char* sv_connect(char* s,data_server*);

int sv_store();

void* sv_retrieve();

int sv_delete();

int sv_disconnect();

#endif

// create server data e free server data
//inizializzare le lock
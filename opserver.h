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


data_server* create_dataserver();

void free_data_server(data_server*);

char* sv_connect(char* s,data_server*);

char* sv_store(char*s,char*objame,data_server* data,char* msg, unsigned int size_msg);

char* sv_retrieve(char* s,char* objname);

char* sv_delete(char* s,char* objname,data_server* data);

char* sv_disconnect(char* s,data_server*);

#endif

#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include"info.h"
#include"hashtable.h"
#include<string.h>
#include"opserver.h"
#include<pthread.h>

#ifndef _opserver_c_
#define _opserver_c_

char* sv_connect(char*s,data_server* data){
    if(strlen(s)>LENNAME){
        return "KO Name too long\n";
    }
    if(strlen(s)<=0){
        return "KO Invalid name\n";
    }
    unsigned int i=hash(s)%N_LOCK;
    int esito=0;

    CHECKLOCK(pthread_mutex_lock(&(data->hashlocks[i])),"KO fail\n");
    esito=insert(s,data->client_connessi);
    CHECKUNLOCK(pthread_mutex_unlock(&(data->hashlocks[i])));

    if(esito==-1) return "KO Name already taken\n";
    else{
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_client_objstore)++;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK\n";
    }
}



#endif
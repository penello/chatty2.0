#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include"info.h"
#include"hashtable.h"
#include<string.h>
#include"opserver.h"
#include<pthread.h>
#include<sys/stat.h>
#include<unistd.h>


#ifndef _opserver_c_
#define _opserver_c_


data_server* create_dataserver(){
    data_server* stat_server=NULL;
    CHECKMALLOC(stat_server=malloc(sizeof(data_server)));
    memset(stat_server,0,sizeof(data_server));
    CHECKLOCKINIT(pthread_mutex_init(&((stat_server)->lock),NULL));
    for(int i=0;i<N_LOCK;i++){
        CHECKLOCKINIT(pthread_mutex_init(&(stat_server->hashlocks[i]),NULL));
    }
    return stat_server;
}

void free_data_server(data_server* stat_server){
    free(stat_server);
}


//controlla la stringa e registra il client
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

        char* directory=NULL;
        CHECKMALLOC(directory=malloc(256*sizeof(char)));
        strncpy(directory,DIRSTORE,256);
        strncat(directory,s,(256-sizeof(DIRSTORE)));
        mkdir(directory,0700);
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_client_objstore)++;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK\n";
    }
}

//controlla la stringa e rimuove il client
char* sv_disconnect(char* s, data_server* data){
    unsigned int i=hash(s)%N_LOCK;
    int esito=0;

    CHECKLOCK(pthread_mutex_lock(&(data->hashlocks[i])),"KO fail\n");
    esito=rimozione(s,data->client_connessi);
    CHECKUNLOCK(pthread_mutex_unlock(&(data->hashlocks[i])));

    if(esito==-1) return "KO name non present\n";
    else{
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_client_objstore)--;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK\n";
    }
}//da modificare per eliminare anche le directory e i file


//elimina il file passato come parametro
char* sv_delete(char* s,char* objname,data_server* data){
    FILE* fp;
    int size=0;
    char* obj_name=NULL;
    CHECKMALLOC(obj_name=malloc(LENOBJ*sizeof(char)));
    strncpy(obj_name,objname,LENOBJ);
    char* directory=NULL;
    CHECKMALLOC(directory=malloc(256*sizeof(char)));
    strncpy(directory,DIRSTORE,256);
    strncat(directory,s,(256-sizeof(DIRSTORE)));
    strcat(directory,"/");
    strncat(directory,obj_name,((256-sizeof(DIRSTORE))-sizeof(s)));

    fp=fopen(directory,"r");
    if(fp==NULL){
        fprintf(stderr,"File not present\n");
        return "KO File not present\n";
    }
    else{
        fseek(fp,0,SEEK_END);
        size=ftell(fp);
        fclose(fp);
        CHECKREMOVEFILE(remove(directory));
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_obj_objstore)--;
        (data->size_objstore)=(data->size_objstore)-size;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK\n";

    }

}

#endif
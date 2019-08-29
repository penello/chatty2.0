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
#include"server.h"


#ifndef _opserver_c_
#define _opserver_c_


data_server* create_dataserver(){
    data_server* stat_server=NULL;
    CHECKMALLOC(stat_server=malloc(sizeof(data_server)));
    memset(stat_server,0,sizeof(data_server));
    stat_server->client_connessi=createhashtable();
    CHECKLOCKINIT(pthread_mutex_init(&((stat_server)->lock),NULL));
    CHECKMALLOC((stat_server->hashlocks)=malloc(N_LOCK*sizeof(pthread_mutex_t)));
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
        int flag=chdir(directory);
        if(flag==-1){
        CHECKMKDIR(mkdir(directory,0700));
        }
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_client_objstore)++;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK \n";
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
        return "OK \n";
    }
}


//elimina il file passato come parametro
char* sv_delete(char* s,char* objname,data_server* data){
    FILE* fp;
    unsigned int size=0;
    char* obj_name=NULL;
    CHECKMALLOC(obj_name=malloc(LENOBJ*sizeof(char)));
    memset(obj_name,0,LENOBJ*sizeof(char));
    strncpy(obj_name,objname,LENOBJ);
    char* directory=NULL;
    CHECKMALLOC(directory=malloc(256*sizeof(char)));
    memset(directory,0,256*sizeof(char));

    strncpy(directory,DIRSTORE,256);
    strncat(directory,s,(256-sizeof(DIRSTORE)));
    strncat(directory,"/",(256-sizeof(DIRSTORE)-1));
    strncat(directory,obj_name,((256-sizeof(DIRSTORE)-(sizeof(s)-1))));

    fp=fopen(directory,"r");
    if(fp==NULL){
        fprintf(stderr,"File not present\n");
        printf("%s\n",directory);
        return "KO File not present\n";
    }
    else{
        fseek(fp,0,SEEK_END);
        size=ftell(fp);
        CHECKFCLOSE(fclose(fp));
        CHECKREMOVEFILE(remove(directory));
        free(obj_name);
        free(directory);
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_obj_objstore)--;
        (data->size_objstore)=(data->size_objstore)-size;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK \n";

    }

}

char* sv_store(char*s,char*objname,data_server* data,char* msg, unsigned int size_msg){

    fprintf(stdout,"sono nella store\n");
    
    FILE* fp;
    fp=malloc(sizeof(FILE*));
    FILE* wr;
    wr=malloc(sizeof(FILE*));
    char* obj_name=NULL;
    CHECKMALLOC(obj_name=malloc(LENOBJ*sizeof(char)));
    memset(obj_name,0,LENOBJ*sizeof(char));
    strncpy(obj_name,objname,LENOBJ);
    char* path_fileame=NULL;
    CHECKMALLOC(path_fileame=malloc(256*sizeof(char)));
    memset(path_fileame,0,256*sizeof(char));

    fprintf(stdout,"dopo un po' di malloc\n");

    strncpy(path_fileame,DIRSTORE,256);
    strncat(path_fileame,s,(256-sizeof(DIRSTORE)));
    strncat(path_fileame,"/",(256-sizeof(DIRSTORE)-1));
    strncat(path_fileame,obj_name,((256-sizeof(DIRSTORE)-(sizeof(s)-1))));

    fprintf(stdout,"faccio la fopen\n");

    fp=fopen(path_fileame,"r");
    if(fp!=NULL){
        fprintf(stderr,"File name already present\n");
        return "KO file name already present\n";
    }
    else{

        wr=fopen(path_fileame,"w");
        if(wr==NULL){
            fprintf(stderr,"Error in create file\n");
            return "KO error in create file\n";
        }
        else{
            fprintf(stdout,"faccio la fwrite\n");
            printf("\nroba della store%s\n%s\n%s\n%s\n%u\n",s,objname,msg,path_fileame,size_msg);

            fwrite(msg,sizeof(char),size_msg,wr);
            CHECKFCLOSE(fclose(wr));

            fprintf(stdout,"faccio la fclose\n");

            free(obj_name);
            free(path_fileame);

            fprintf(stdout,"dopo free prima di lock\n");

            CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
            (data->n_obj_objstore)++;
            (data->size_objstore)=(data->size_objstore)+size_msg;
            CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));

            fprintf(stdout,"dopo lock\n");

            return "OK \n";
        }


    }

}

char* sv_retrieve(char* s,char* objname){
    FILE* fp;
    unsigned int size=0;
    char* obj_name=NULL;
    CHECKMALLOC(obj_name=malloc(LENOBJ*sizeof(char)));
    memset(obj_name,0,LENOBJ*sizeof(char));
    strncpy(obj_name,objname,LENOBJ);
    char* directory=NULL;
    CHECKMALLOC(directory=malloc(256*sizeof(char)));
    memset(directory,0,256*sizeof(char));

    strncpy(directory,DIRSTORE,256);
    strncat(directory,s,(256-sizeof(DIRSTORE)));
    strncat(directory,"/",(256-sizeof(DIRSTORE)-1));
    strncat(directory,obj_name,((256-sizeof(DIRSTORE)-(sizeof(s)-1))));

    fp=fopen(directory,"r");
    if(fp==NULL){
        fprintf(stderr,"File not present\n");
        return "KO File not present\n";
    }
    else{
        fseek(fp,0,SEEK_END);
        size=ftell(fp);
        fseek(fp,0,SEEK_SET);
        char* data_size=NULL;
        CHECKMALLOC(data_size=malloc(16*sizeof(char)));
        memset(data_size,0,16*sizeof(char));
        sprintf(data_size,"%d",size);
        char* return_data=NULL;
        CHECKMALLOC(return_data=malloc(100100*sizeof(char)));
        memset(return_data,0,100100*sizeof(char));
        strncpy(return_data,"DATA ",100100);
        strncat(return_data,data_size,100090);
        strncat(return_data," \n ",100080);
        char ch;
        unsigned int i=0;
        char* temp=NULL;
        CHECKMALLOC(temp=malloc(100001*sizeof(char)));
        memset(temp,0,100001*sizeof(char));
        while( (ch = getc(fp)) != EOF ) {
            temp[i]=ch;
            i++;
        }
        strncat(return_data,temp,100050);
        CHECKFCLOSE(fclose(fp));
        free(obj_name);
        free(directory);
        free(data_size);
        free(temp);
        return return_data;
    }
}


#endif
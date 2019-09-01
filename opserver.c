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

//inizializza la struct presente nell'omonimo file.h
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


//libera la memoria allocata alla struct dell'omonimo file.h
void free_data_server(data_server* stat_server){
    free(stat_server->hashlocks);
    free_hashtable(stat_server->client_connessi);
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
        if (mkdir(s,0777) == -1) {
		    if (errno != EEXIST) {
			    perror("worker creating directory");
                    CHECKLOCK(pthread_mutex_lock(&(data->hashlocks[i])),"KO fail\n");
                    esito=rimozione(s,data->client_connessi);
                    CHECKUNLOCK(pthread_mutex_unlock(&(data->hashlocks[i])));
                return "KO an error occured\n";
		    }
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
    int len=strlen(s);
    char path[LENNAME];
    strncpy(path,s,50);
    path[len]='/';
    strncat(path,objname,50);
    
    fp=fopen(path,"r");
    if(fp==NULL){
        fprintf(stderr,"File not present\n");
        printf("%s\n",path);
        return "KO File not present\n";
    }
    else{
        fseek(fp,0,SEEK_END);
        size=ftell(fp);
        CHECKFCLOSE(fclose(fp));
        int ok=0;
        ok=unlink(path);
        if(ok==-1){
            fprintf(stderr,"Error unlik\n");
            return "KO error unlink\n";
        }
        CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
        (data->n_obj_objstore)--;
        (data->size_objstore)=(data->size_objstore)-size;
        CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));
        return "OK \n";

    }

}

//salva il file passato come parametro
char* sv_store(char*s,char*objname,data_server* data,char* msg, unsigned int size_msg){

    
    FILE* fp;
    fp=NULL;
    FILE* wr;
    wr=NULL;

    int len=strlen(s);
    char path[LENNAME];
    strncpy(path,s,50);
    path[len]='/';
    strncat(path,objname,50);

    fp=fopen(path,"r");
    if(fp!=NULL){
        fprintf(stderr,"File name already present\n");
        CHECKFCLOSE(fclose(fp));
        return "KO file name already present\n";
    }
    else{

        wr=fopen(path,"w");
        if(wr==NULL){
            fprintf(stderr,"Error in create file\n");
            return "KO error in create file\n";
        }
        else{
            fwrite(msg,sizeof(char),size_msg,wr);
            CHECKFCLOSE(fclose(wr));


            CHECKLOCK(pthread_mutex_lock(&(data->lock)),"KO fail\n");
            (data->n_obj_objstore)++;
            (data->size_objstore)=(data->size_objstore)+size_msg;
            CHECKUNLOCK(pthread_mutex_unlock(&(data->lock)));

            return "OK \n";
        }
    }
}

//ritorna il file con il nome passato come parametro
char* sv_retrieve(char* s,char* objname){
    FILE* fp;
    unsigned int size=0;
    int len=strlen(s);
    char path[LENNAME];
    strncpy(path,s,50);
    path[len]='/';
    strncat(path,objname,50);

    fp=fopen(path,"r");
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
        strncpy(return_data,"DATA ",6);
        strncat(return_data,data_size,16);
        strncat(return_data," \n ",4);
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
        free(data_size);
        free(temp);
        return return_data;
    }
}


#endif
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
#include<unistd.h>

void leave(int fd,char*s,connections* stats,int* stop){
    char buff[8];
    int a;
    CHECKREAD(a=read(fd,buff,8));
    if(strncmp(buff,"EAVE \n",8)==0){
        char result[LENNAME];
        strncpy(result,sv_disconnect(s,(stats->sv_stats)),LENNAME);
        CHECKWRITE(write(fd,result,LENNAME));
        *stop=1;
    }
    else{
        CHECKWRITE(write(fd,"KO An error occured, non valid leave request\n",LENNAME));
        *stop=1;
        return;
    }
}


void _delete(int fd,char*s,connections* stats,int* stop){
    char buff[LENNAME];
    int a;
        CHECKREAD(a=read(fd,buff,LENNAME));
        if(strncmp(buff,"ELETE ",6)!=0 || strlen(buff)<9){
            CHECKWRITE(write(fd,"KO an error occured, delete request non valid\n",LENNAME));
            *stop=1;
            return;
        }
        char* obj;
        char result[LENNAME];
        obj=buff;
        obj=strtok(obj," ");
        obj=strtok(NULL," ");
        strncpy(result,sv_delete(s,obj,(stats->sv_stats)),LENNAME);
        CHECKWRITE(write(fd,result,LENNAME));
}


void store(int fd,char*s,connections* stats,int*stop){

    fprintf(stdout,"Abbiamo una richiesta di store\n");

    char buff[100100];
    int a=0;

    fprintf(stdout,"Vediamo se è una vera richiesta di store\n");

    CHECKREAD(a=read(fd,buff,100100));
    if(strncmp(buff,"TORE ",5)!=0 || strlen(buff)<11){
        CHECKWRITE(write(fd,"KO an error occured, invalid store request",LENNAME));
        *stop=1;
        return;
    }
    
    fprintf(stdout,"inizializzo per la store\n");

    printf("messaggio prima delle strtok:\n%s\n",buff);

    char* string;
    string=buff+5;
    string=strtok(string," ");
    char* nameobj;
    nameobj=string;
    string=strtok(NULL," ");
    char* len;
    len=string;
    string=strtok(NULL," ");
    char *msg=string+2;


    if(strlen(msg)==0){
        fprintf(stdout,"Msg vuoto\n");
        CHECKWRITE(write(fd,"KO Msg non valid\n",LENNAME));
        *stop=1;
    }
    else{
    printf("messaggio prima della chiamata a sv_store:\n%s\n",msg);

    fprintf(stdout,"trasformo il len mex in unsigned int\n");

    unsigned int len_msg=strtoul(len,NULL,10);

    char result[LENNAME];

    fprintf(stdout,"trasformato il len mex in unsigned int\n");
    

    strncpy(result,sv_store(s,nameobj,(stats->sv_stats),msg,len_msg),LENNAME);

    printf("%s STORE\n",result);

    CHECKWRITE(write(fd,result,LENNAME));
    }

}


void retrive(int fd,char*s,int* stop){
    char buff[LENNAME];
    int a;
    CHECKREAD(a=read(fd,buff,LENNAME));
    if(strncmp(buff,"ETRIEVE ",8)!=0 || strlen(buff)<11){
        CHECKWRITE(write(fd,"KO an error occured, invalid retrieve request",LENNAME));
        *stop=1;
        return;
    }

    char* name_obj;
    name_obj=buff+8;
    name_obj=strtok(name_obj," ");

    char result[100100];
    strncpy(result,sv_retrieve(s,name_obj),LENNAME);
    CHECKWRITE(write(fd,result,LENNAME));

}



void* worker(connections* stats){
    int cs=stats->fd;
    char temp[LENNAME];
    char result[LENNAME];
    int a=0;

    fprintf(stdout,"adesso leggo la richiesta\n");

    CHECKREAD(a=read(cs,temp,LENNAME));
    if(a==0){
        CHECKWRITE(write(cs,"KO during trasmission\n",LENNAME));
        pthread_exit(NULL);
    }
    if(strncmp(temp,"REGISTER ",9)!=0 || strlen(temp)<12){
        CHECKWRITE(write(cs,"KO invalid registration message \n",LENNAME));
        pthread_exit(NULL);
    }

    fprintf(stdout,"HO letto la richiesta ed è una register\n");

    char* name;
    name=temp+9;
    name=strtok(name," ");

    strncpy(result,sv_connect(name,(stats->sv_stats)),LENNAME);
    if(strncmp(result,"OK \n",LENNAME)!=0){
        CHECKWRITE(write(cs,"KO an error occured in register request\n",LENNAME));
        close(cs);
        pthread_exit(NULL);
    }
    
    printf("%s REGISTER\n",result);

    CHECKWRITE(write(cs,result,LENNAME));

    char s;
    int stop=0;
    while(read(cs,&s,1) && !stop){
        switch (s){
            case 'S':
                    store(cs,name,stats,&stop);
                    break;
            case 'R':
                    retrive(cs,name,&stop);
                    break;
            case 'D':
                    _delete(cs,name,stats,&stop);
                    break;
            case 'L':
                    leave(cs,name,stats,&stop);
                    break;
        }
    }

    fprintf(stdout,"Chiudo comunicazione\n");
    close(cs);
    pthread_exit(NULL);
    
}
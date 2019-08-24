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

void leave(int fd,char*s,int* stop){
    char buff[8];
    int a;
    CHECKREAD(a=read(fd,buff,8));
    if(strncmp(buff,"EAVE \n",8)==0){
        char result[LENNAME];
        strncpy(result,sv_disconnect(s,stats),LENNAME);
        CHECKWRITE(write(fd,result,LENNAME));
        *stop=1;
    }
    else{
        CHECKWRITE(write(fd,"KO An error occured, non valid leave request\n",LENNAME));
        *stop=1;
    }
}


void delete(int fd,char*s,int* stop){
    char buff[LENNAME];
    int a;
        CHECKREAD(a=read(fd,buff,LENNAME));
        if(strncmp(buff,"ELETE ",6)!=0 || strlen(buff)<9){
            CHECKWRITE(write(fd,"KO an error occured, delete request non valid\n",LENNAME));
            *stop=1;
        }
        char* obj;
        char result[LENNAME];
        obj=buff+6;
        obj=strtok(buff," ");
        strncpy(result,sv_delete(s,obj,stats),LENNAME);
        CHECKWRITE(write(fd,result,LENNAME));
}


void store(int fd,char*s,int*stop){
    char buff[100100];
    int a;
    CHECKREAD(a=read(fd,buff,100100));
    if(strncmp(buff,"TORE ",5)!=0 || )
}



static void* worker(void* arg){
    int cs=*((int*)arg);
    char temp[LENNAME];
    char result[LENNAME];
    int a;

    CHECKREAD(a=read(cs,temp,LENNAME));
    if(a==0){
        CHECKWRITE(write(cs,"KO during trasmission\n",LENNAME));
        pthread_exit(NULL);
    }
    if(strncmp(temp,"REGISTER ",9) || strlen(temp)<12){
        CHECKWRITE(write(cs,"KO invalid registration message \n",LENNAME));
        pthread_exit(NULL);
    }

    char* name;
    char* delim="\n";
    name=temp+9;
    name=strtok(name," ");

    strncpy(result,sv_connect(name,stats),LENNAME);
    CHECKWRITE(write(cs,result,LENNAME));

    char s;
    int stop=0;
    while(read(cs,&s,1) && !stop){
        switch (s){
            case 'S':
                    store(cs,name,&stop);
                    break;
            case 'R':
                    retrive(cs,name,&stop);
                    break;
            case 'D':
                    delete(cs,name,&stop);
                    break;
            case 'L':
                    leave(cs,name,&stop);
                    break;
        }
    }

    close(cs);
    pthread_exit(NULL);
}
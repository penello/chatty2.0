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
#include"libclient.h"

#ifndef _libclient_c
#define _libclient_c

int os_connect(char* name){
    struct sockaddr_un sa;
	strncpy(sa.sun_path, SOCKNAME, 108);
	sa.sun_family = AF_UNIX;

    CHECKSOCKET(socket_cl=socket(AF_UNIX,SOCK_STREAM,0));
    CHECKCONNECT(connect(socket_cl,(struct sockaddr*)&sa,sizeof(sa)));

    char op[LENNAME]="REGISTER ";
    strncat(op,name,50);
    strncat(op," \n",3);

    CHECKWRITE(write(socket_cl,op,LENNAME));
    char* buff;
    CHECKMALLOC(buff=malloc(LENNAME*sizeof(char)));
    memset(buff,0,LENNAME*sizeof(char));
    int a=0;
    CHECKREAD(a=read(socket_cl,buff,LENNAME));

    if(strncmp(buff,"OK \n",LENNAME)==0){
        return 1;
    }
    else{
        close(socket_cl);
        return 0;
    }

}

int os_store(char *name, void *block, size_t len){
    char op[100100]="STORE ";
    char msg_dim[10];
    sprintf(msg_dim, "%ld", len);
    strncat(op,name,50);
    strncat(op," ",2);
    strncat(op,msg_dim,10);
    strncat(op," ",2);
    strncat(op,"\n",2);
    strncat(op," ",2);
    strncat(op,block,100000);

    CHECKWRITE(write(socket_cl,op,100100));
    char* buff;
    CHECKMALLOC(buff=malloc(LENNAME*sizeof(char)));
    memset(buff,0,LENNAME*sizeof(char));
    int a=0;
    CHECKREAD(a=read(socket_cl,buff,LENNAME));

    if(strncmp(buff,"OK \n",LENNAME)==0){
        return 1;
    }
    else return 0;

}

void *os_retrieve(char *name){
    char op[LENNAME]="RETRIEVE ";
    strncat(op,name,50);
    strncat(op," \n",3);

    CHECKWRITE(write(socket_cl,op,LENNAME));
    int a=0;
    char* buff;
    CHECKMALLOC(buff=malloc(100100*sizeof(char)));
    memset(buff,0,100100*sizeof(char));
    CHECKREAD(a=read(socket_cl,buff,100100));

    if(strncmp(buff,"DATA ",5)==0 && strlen(buff)>7){
        char* string;
        string=buff+5;
    
        char spazio[2]=" \0";
        int i=0;
        int count=0;
        while(count!=2){
            if(string[i]==spazio[0]){count++;}
            i++;
        }
        char* msg;
        msg=string+i;

    return msg;
    }
    else return NULL;
}


int os_delete(char *name){
    char op[LENNAME]="DELETE ";
    strncat(op,name,50);
    strncat(op," ",2);
    strncat(op,"\n",2);

    CHECKWRITE(write(socket_cl,op,LENNAME));
    int a=0;
    char* buff;
    CHECKMALLOC(buff=malloc(LENNAME*sizeof(char)));
    memset(buff,0,LENNAME*sizeof(char));
    CHECKREAD(a=read(socket_cl,buff,LENNAME));

    if(strncmp(buff,"OK \n",LENNAME)==0){
        return 1;
    }
    else return 0;
}

int os_disconnect(){
    char op[LENNAME]="LEAVE \n";
    char* buff;
    CHECKMALLOC(buff=malloc(LENNAME*sizeof(char)));
    memset(buff,0,LENNAME*sizeof(char));
    CHECKWRITE(write(socket_cl,op,LENNAME));
    int a=0;
    CHECKREAD(a=read(socket_cl,buff,LENNAME));

    if(strncmp(buff,"OK \n",LENNAME)==0){
        return 1;
    }
    else return 0;
}


#endif
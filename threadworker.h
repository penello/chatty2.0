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

#ifndef _threadworker_h_
#define _threadworker_h


void leave(int fd,char*s,connections* stats,int* stop);

void _delete(int fd,char*s,connections* stats,int* stop);

void store(int fd,char*s,connections* stats,int*stop);

void retrive(int fd,char*s,int* stop);

void* worker(void* arg);

#endif
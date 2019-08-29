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

#ifndef _libclient_h
#define _libclient_h

int socket_cl;

int os_connect(char *name);

int os_store(char *name, void *block, size_t len);

void *os_retrieve(char *name);

int os_delete(char *name);

int os_disconnect();

#endif
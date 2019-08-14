#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include"info.h"


#ifndef _hashtable_h_
#define _hashtable_h_


typedef struct nodoclient{
    char name [LENNAME];
    struct nodoclient* next;
} node;

node** createhashtable();

unsigned int hash(char*s);

int insert(char*s,node** list);

int rimozione(char *s,node** ht);

void free_hashtable(node**ht);

#endif

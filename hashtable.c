#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include"hashtable.h"
#include"info.h"
#include<string.h>
#include <limits.h>

#ifndef _hashtable_c_
#define _hashtable_c_

#define BITS_IN_int     ( sizeof(int) * CHAR_BIT )
#define THREE_QUARTERS  ((int) ((BITS_IN_int * 3) / 4))
#define ONE_EIGHTH      ((int) (BITS_IN_int / 8))
#define HIGH_BITS       ( ~((unsigned int)(~0) >> ONE_EIGHTH ))

node** createhashtable(){
    node** ht=NULL;
    CHECKMALLOC(ht=malloc(DIMHASHTABLE*sizeof(node*)));
    memset(ht,0,DIMHASHTABLE*sizeof(node*));
    return ht;
}

void free_hashtable(node** ht){
    for(int i=0;i<DIMHASHTABLE;i++){
        node* corr=ht[i];
        while(corr!=NULL){
            node* tmp=corr;
            corr=corr->next;
            free(tmp);
        }
    }
    free(ht);
}


// la funzione di hashing è palesemente copiata dall'assegnamento 9
unsigned int hash(char* s){
     char *datum = (char *)s;
    unsigned int hash_value, i;

    if(!datum) return 0;

    for (hash_value = 0; *datum; ++datum) {
        hash_value = (hash_value << ONE_EIGHTH) + *datum;
        if ((i = hash_value & HIGH_BITS) != 0)
            hash_value = (hash_value ^ (i >> THREE_QUARTERS)) & ~HIGH_BITS;
    }
    return (hash_value)%DIMHASHTABLE;
}


int insert(char*s,node** ht){
    int index=hash(s);
    node* new=NULL;
    CHECKMALLOC(new=malloc(sizeof(node)));
    memset(new,0,sizeof(node));
    strncpy(new->name,s,LENNAME);
    if(ht[index]==NULL)
        ht[index]=new;
    else{
        node* precc=NULL;
        node* corr=ht[index]; 
        while(corr!=NULL){
            if(strncmp(s,corr->name,LENNAME)==0){
                fprintf(stdout,"Name already present\n");
                free(new);
                return -1;
            }
            else{
                precc=corr;
                corr=corr->next;
            }
        }
        precc->next=new;
    }
    return 1;
}

int rimozione(char *s,node** ht){
    int index=hash(s);
    node* precc=NULL;
    node* corr=ht[index];
    while(corr!=NULL){
        if(strncmp(corr->name,s,LENNAME)==0){
            if(precc==NULL){
                ht[index]=corr->next;
                free(corr);
                return 1;
            }
            else{
                precc->next=corr->next;
                free(corr);
                return 1;
            }
        }
            else{
                precc=corr;
                corr=corr->next;
        }
    }
    fprintf(stdout,"Name not present\n");
    return -1;
}

#endif
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

#define t "store/ciao/*"
#define p "exec rm -r "

int main(){
    
    char* o=malloc(1024*sizeof(char));
    char* d=malloc(256*sizeof(char));
    d=strcpy(d,"store/ciao");
    //fprintf(stdout,"creo cartella\n");
    //mkdir(d,0700);
    strcat(o,p);
    strcat(o,t);
    fprintf(stdout,"elimino cartella\n");
    system(d);
    rmdir(d);
    return 0;
}
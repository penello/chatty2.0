
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "opserver.h"

int main() {
    /*printf("inizio bello\n");
    data_server* lol=create_dataserver();
    printf("creato data server\n");
    char* connect=sv_connect("mario",lol);
    printf("sv_connect fatta\n");
    char *big = (char*)malloc(100000000);
    memset(big, '1', 100000000);
    printf("big creato\n");
    
    char* store=sv_store("mario","big",lol,big,100000000);
    printf("store fatta\n");

    char *retrbig = sv_retrieve("mario","big");
    if (strcmp(retrbig, big) == 0) printf("I blocchi sono uguali\n");
    else {
        printf("I blocchi sono diversi\n");
        printf("%s\n",retrbig);
    }
    printf("stats:\n");
    printf("%d\n",lol->n_client_objstore);
    printf("%d\n",lol->n_obj_objstore);
    printf("%d\n",lol->size_objstore);
    char* remove=sv_delete("mario","big",lol);
    printf("stats:\n");
    printf("%d\n",lol->n_client_objstore);
    printf("%d\n",lol->n_obj_objstore);
    printf("%d\n",lol->size_objstore);
    sv_disconnect("mario",lol);
    printf("stats:\n");
    printf("%d\n",lol->n_client_objstore);
    printf("%d\n",lol->n_obj_objstore);
    printf("%d\n",lol->size_objstore);
    
    free(big);*/

    char* name;
    char* delim="\n";
    char temp[80]="REGISTER Mariooo\n cioccolata";
    name=temp;
    //name=strtok(name," ");
    name=strtok(name,delim);
    name=strtok(NULL," ");
    printf("%s",name);

    return 0;
}
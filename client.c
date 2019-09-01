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
#include"libclient.h"

int succop=0;
int optentate=0;

void store(){
    int i,j=0;
    int dim=100;
    char temp[100000];
    char name[3];
    for(i=0;i<20;i++){
        while(j+4<dim){
            strncpy(temp+j,"aaaa",5);
            j+=4;
        }
        temp[j]='\0';
        sprintf(name,"%d%d", i/10, i%10);
        optentate++;
        succop+=os_store(name,"123",4);
        dim+=5257;
    }
}

void retrieve () {
	int i, j = 0;
	int dim = 100;
	char temp[100000];
	char name[3];
	char* ret;
	for(i=0;i<20;i++) {
		while (j+4<dim) {
			strncpy(temp+j,"aaaa",5);
			j+=4;
		}
		temp[j] = '\0';
		sprintf(name,"%d%d", i/10, i%10);
		optentate++;
		ret = os_retrieve(name);
		if (strncmp(ret,"123",1) == 0)
            succop++;
		dim += 5257;
	}
}

void delete () {
	int i;
	char name[3];
	for(i=0;i<20;i++) {
		sprintf(name,"%d%d", i/10, i%10);
		optentate++;
		succop += os_delete(name);
	}
}

int main(int argc,char* argv[]){
    int test;
    char name[LENNAME]="\0";
    sscanf(argv[1],"%s",name);
    sscanf(argv[2],"%d",&test);

    if (strlen(name) == 0 || test < 1 || test > 3) {
		printf("ERR: usage = client [NAME] [TESTCODE], where 0 < TESTCODE:int < 4\n");
		return 1;
    }

    int succ=os_connect(name);
    if(succ==0){
        printf("Error durig connect\n");
        return 0;
    }
    else{
        optentate++;
        succop++;
    }

    switch (test){
        case 1:
            store();
            break;
        case 2:
            retrieve();
            break;
        case 3:
            delete();
            break;
    }
    optentate++;
    succop+= os_disconnect();

    printf("\nTEST %d : %d successful operations on %d\n", test, succop, optentate);
	fflush(stdout);

return 0;

    return 0;
}
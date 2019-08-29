#ifndef _info_h_
#define _info_h_

#define LENNAME 101
#define DIMHASHTABLE 1024
#define N_LOCK 128
#define BACKLOG 512
#define DIRSTORE "store/"
#define LENOBJ 101
#define SOCKNAME "./objstore.sock"


#define CHECKMALLOC(call) if((call)==NULL){ fprintf(stderr,"Error Malloc\n"); exit(EXIT_FAILURE);}

#define CHECKMKDIR(X) if((X)==-1) {fprintf(stderr,"Error: create directory\n"); pthread_exit(NULL);}

#define CHECKREAD(call) if((call)==-1) {fprintf(stderr,"Error in read\n"); pthread_exit(NULL);}

#define CHECKWRITE(X) if((X)==-1){fprintf(stderr,"Error in write\n"); pthread_exit(NULL);};

#define CHECKLOCKINIT(X) if((errno=(X))){fprintf(stderr,"Error lock init\n");}

#define CHECKTHREAD(X) if((X)!=0) fprintf(stderr,"Error in create thread\n");

#define CHECKSOCKET(call) if((call)==-1) fprintf(stdout,"Error create Socket\n");

#define CHECKCONNECT(X) if((X)==-1) fprintf(stdout,"Error in socket connect\n");

#define CHECKBIND(X) if((X)==-1) fprintf(stderr,"Error in bind\n");

#define CHECKLISTEN(X) if((X)==-1) fprintf(stderr,"Error in socketlisten\n");

#define CHECKLOCK(call,ret) if((call)!=0) { fprintf(stderr,"lock failed\n"); return ret; }

#define CHECKACCEPT(call) if((call)==-1) fprintf(stderr,"Error in accept connection\n");

#define CHECKUNLOCK(call) if((call)!=0) fprintf(stderr,"unlock failed\n");

#define CHECKREMOVEFILE(X) if((X)!=0){ fprintf(stderr,"Remove file:ERROR\n"); exit(EXIT_FAILURE);}

#define CHECKFCLOSE(X) if((X)!=0) { fprintf(stderr,"Error in close file\n");}

#endif
#ifndef _info_h_
#define _info_h_

#define LENNAME 101
#define DIMHASHTABLE 1024
#define N_LOCK 128
#define BACKLOG 512
#define DIRSTORE "store/"
#define LENOBJ 101

#define CHECKMALLOC(call) if((call)==NULL){ fprintf(stderr,"Error Malloc\n"); exit(EXIT_FAILURE);}

#define CHECKLOCKINIT(X) if((errno=(X))){fprintf(stderr,"Error lock init\n");}

//#define CHECKSOCKET(call) if((call)==-1) fprintf(stdout,"Socket error\n");

#define CHECKLOCK(call,ret) if((call)!=0) { fprintf(stderr,"lock failed\n"); return ret; }

#define CHECKUNLOCK(call) if((call)!=0) fprintf(stderr,"unlock failed\n");

#define CHECKREMOVEFILE(X) if((X)!=0){ fprintf(stderr,"Remove file:ERROR\n"); exit(EXIT_FAILURE);}
#endif
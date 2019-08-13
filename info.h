#ifndef _info_h_
#define _info_h_

#define LENNAME 101
#define DIMHASHTABLE 1024
#define N_LOCK 128

#define CHECKMALLOC(call) if((call)==NULL){ fprintf(stderr,"Error Malloc\n"); exit(EXIT_FAILURE);}

//#define CHECKSOCKET(call) if((call)==-1) fprintf(stdout,"Socket error\n");

#define CHECKLOCK(call,ret) if((call)!=0) { fprintf(stderr,"lock failed\n"); return ret; }
#define CHECKUNLOCK(call) if((call)!=0) fprintf(stderr,"unlock failed\n");

#endif
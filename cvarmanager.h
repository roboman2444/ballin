#ifndef CVARMANAGERHEADER
#define CVARMANAGERHEADER

struct cvar_s;
typedef void (* cvarcallback_t)(struct cvar_s *c);

typedef struct cvar_s {
	int type;
	int myid;
	char * name;

	char * helptext; //maybe
	char * defaultstring;

	int valueint;
	float valuefloat;
	float valuevector[3];
	char * value;

	char isdefault; //todo ehh


	int numcallbacks;
	cvarcallback_t *onchanges;

} cvar_t;


typedef struct cvarlist_s {
	cvar_t **list;
	int count;
} cvarlist_t;

#include "idlist.h"

IDLIST_HEADER(cvar, cvar_t, cvarlist_t);


int cvar_init(void);

int cvar_unload(cvar_t *c);
int cvar_shutdown(void);

#endif

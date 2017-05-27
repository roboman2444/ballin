#ifndef MODELMANAGERHEADER
#define MODELMANAGERHEADER
typedef struct model_s {
	int type;
	int myid;
	char * name;
} model_t;

typedef struct modellist_s {
	model_t **list;
	int count;
} modellist_t;

#include "idlist.h"

IDLIST_HEADER(model, model_t, modellist_t);

int model_init(void);


#endif

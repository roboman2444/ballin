#ifndef MODELMANAGERHEADER
#define MODELMANAGERHEADER
typedef struct model_s {
	int type; //1 is registered, 2 is model loaded (kindof), 3 is vbos setup, 4 is postprocessing done
		//type should really only ever be 1 or 4
	int myid;
	char * name;
	vbo_t mvbo; //not tracked by the idlist

	float * interleaveddata;
} model_t;

typedef struct modellist_s {
	model_t **list;
	int count;
} modellist_t;

#include "idlist.h"
//DO NOT USE _remove manually unless you are SURE about it!
IDLIST_HEADER(model, model_t, modellist_t);

int model_init(void);

int model_register(char * name);

int model_load(model_t *m);

int model_unload(model_t *m);


#endif

#ifndef SHADERNAGERHEADER
#define SHADERMANAGERHEADER
typedef struct shader_s {
	int type;//todo figure out types
	int myid;
	char * name;
	unsigned int programid;
	//todo figure out uniform locations and whatnot
} shader_t;

typedef struct shaderlist_s { //todo move these into idlist?
	shader_t **list;
	int count;
} shaderlist_t;

#include "idlist.h"
//DO NOT USE _remove manually unless you are SURE about it!
IDLIST_HEADER(shader, shader_t, shaderlist_t);

int shader_init(void);

int shader_register(char * name);

int shader_load(shader_t *s);
int shader_unload(shader_t *s);


#endif

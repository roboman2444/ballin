#ifndef VBOMANAGERHEADER
#define VBOMANAGERHEADER


typedef struct vbo_s {
	int type; //todo soon to get rid of
	int myid;
} vbo_t;

#include "idlist.h"

IDLIST_HEADER_NOHASH(vbo, vbo_t);

int vbo_init(void);

#endif

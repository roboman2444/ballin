#ifndef VBOMANAGERHEADER
#define VBOMANAGERHEADER
#include "vertexattribs.h"

//standard vbo, interleaved
typedef struct vbo_s {
	int type;
	int myid;

	unsigned int vaoid;
	unsigned int vertsid;
	unsigned int facesid; //indices
	int numverts;
	int numfaces;

	int totalstride;

	unsigned char stridewidth[MAXATTRIBS]; //will eventually make this longer
} vbo_t;

#include "idlist.h"

IDLIST_HEADER_NOHASH(vbo, vbo_t);

int vbo_init(void);

#endif

#include "globaldefs.h"
#include "vbomanager.h"



IDLIST_INTERNAL_NOHASH(vbo, vbo_t);


int vbo_init(void){
	IDLIST_INIT_NOHASH(vbo, vbo_t);
	return TRUE;
}


IDLIST_CODE_NOHASH(vbo, vbo_t);

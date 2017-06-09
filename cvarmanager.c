#include "globaldefs.h"

#include "stringlib.h"		//for idlist
#include "hashtables.h"		//for idlist

#include "cvarmanager.h"

IDLIST_INTERNAL(cvar, cvar_t, cvarlist_t);
int cvar_init(void){
	//todo
	IDLIST_INIT(cvar, cvar_t, cvarlist_t);
	return TRUE;
}


int cvar_unload(cvar_t *c){
	//todo
	return TRUE;
}

IDLIST_CODE(cvar, cvar_t, cvarlist_t);

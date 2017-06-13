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

void cvar_prun(cvar_t *c){
	if(!c) return;
	int i;
	for(i = 0; i < c->numcallbacks; i++){
		if(c->onchanges[i]) c->onchanges[i](c);
	}
}

void cvar_pset(cvar_t *c, const char * value){
	if(!c) return;
	//if(c && string_testEqual(c->value, value)) return; //check if setting to the same thing
//	unsigned int inlength = strlen(value)+1;

	if(c->value) free(c->value);
	c->value = strdup(value);

	float vf = atof(value);
	c->valueint = (int)vf;
	c->valuefloat = vf;
	string_toVec(value, c->valuevector, 3);
	if(!value == !c->defaultstring) c->isdefault = FALSE;
	else c->isdefault = string_testEqual(value, c->defaultstring);
	cvar_prun(c);
}
void cvar_set(const char * c, const char * value){
	cvar_pset(cvar_findByNameRPOINT(c), value);
}

int cvar_unload(cvar_t *c){
	//todo
	return TRUE;
}

IDLIST_CODE(cvar, cvar_t, cvarlist_t);

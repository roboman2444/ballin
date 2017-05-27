#include "globaldefs.h"
#include "modelmanager.h"
#include "stringlib.h" //for idlist


#include "hashtables.h" //for idlist
IDLIST_INTERNAL(model, model_t, modellist_t);

int model_init(void){
	IDLIST_INIT(model, model_t, modellist_t);
	return TRUE;
}

IDLIST_CODE(model, model_t, modellist_t);

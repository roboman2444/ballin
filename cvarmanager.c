#include "globaldefs.h"

#include "stringlib.h"		//for "idlist"
#include "hashtables.h"		//for "idlist"

#include "cvarmanager.h"


cvar_t ** cvar_list = 0;
int cvar_count = 0;
int cvar_roll = 1;
int cvar_arraysize = 0;
int cvar_arrayfirstopen = 0;
int cvar_arraylasttaken = -1;
int cvar_ok = 0;

hashbucket_t cvar_hashtable[MAXHASHBUCKETS];

int cvar_init(void){
	//todo
	if(cvar_list)free(cvar_list);
	cvar_list	=0;
	cvar_count	=0;
	cvar_roll	=1;
	cvar_arraysize	=0;
	cvar_arraylasttaken =-1;
	memset(cvar_hashtable, 0, MAXHASHBUCKETS * sizeof(hashbucket_t));


	return TRUE;
}

void cvar_prun(cvar_t *c){
	if(!c) return;
	cvarcallback_t *pr = c->onchanges;
	if(!pr) return;
	for(; *pr; pr++) (*pr)(c);
}


void cvar_print(cvar_t *c){
	if(!c) return;

	printf("cvar_print: %i \"%s\", \"%s\", \"%s\", {%i, %f, %f:%f:%f, \"%s\"}\n", c->type, c->name, c->helptext, c->defaultstring,
		c->valueint, c->valuefloat,
		c->valuevector[0], c->valuevector[1], c->valuevector[2],
		c->value);
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
void cvar_idset(const int c, const char * value){
	cvar_pset(cvar_returnById(c), value);
}

//only for manually allocated cvars
int cvar_unload(cvar_t *c){
	//todo
	return TRUE;
}

cvar_t * cvar_findByNameRPOINT(const char * name){
	return cvar_returnById(hash_findByNameRINT(name, cvar_hashtable));
}

int cvar_findByNameRINT(const char * name){
	return hash_findByNameRINT(name, cvar_hashtable);
}


/*
TYPELIST _CONCAT(NAME, _findAllByNameRPOINT)(const char * name){
	TYPELIST ret = {0};
	int hash = hash_get(name);
	hashbucket_t * hb = &_CONCAT(NAME, _hashtable)[hash];
	if(!hb->name) return ret;
	for(; hb; hb = hb->next){
		if(string_testEqual(hb->name, name)){
		ret.count++;
		ret.list = realloc(ret.list, ret.count * sizeof(cvar_t *));
		ret.list[ret.count-1] = _CONCAT(NAME, _returnById)(hb->id);
		}
	}
	return ret;
}

idlist_t _CONCAT(NAME, _findAllByNameRINT)(const char * name){
	idlist_t ret = {0};
	int hash = hash_get(name);
	hashbucket_t * hb = &_CONCAT(NAME, _hashtable)[hash];
	if(!hb->name) return ret;
	for(; hb; hb = hb->next){
		if(string_testEqual(hb->name, name)){
		ret.count++;
		ret.list = realloc(ret.list, ret.count * sizeof(int));
		ret.list[ret.count-1] = hb->id;
		}
	}
	return ret;
}
*/
cvar_t * cvar_returnById(const int id){
        int index = (id & 0xFFFF);
        cvar_t * ret = cvar_list[index];
        if(ret->myid == id) return ret;
        return FALSE;
}

void cvar_pruneList(void){
	if(cvar_arraysize == cvar_arraylasttaken) return;
	cvar_arraysize = cvar_arraylasttaken+1;
	cvar_list = realloc(cvar_list, cvar_arraysize * sizeof(cvar_t *));
}

//TODO
/*
int _CONCAT(NAME, _remove)(const int id){
	int index = (id & 0xFFFF);
	TYPE * ret = &_CONCAT(NAME, _list)[index];
	if(ret->myid != id) return FALSE;
	_CONCAT(NAME, _count)--;
	if(ret->name){
		hash_removeFromTable(ret->name, id, _CONCAT(NAME, _hashtable));
		free(ret->name);
	}
	memset(ret, 0, sizeof(TYPE));//todo just test if setting type/id to 0 is good enough
	if(index < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arrayfirstopen) = index;
	for(; _CONCAT(NAME, _arraylasttaken) > 0 && !_CONCAT(NAME, _list)[_CONCAT(NAME, _arraylasttaken)].myid; _CONCAT(NAME, _arraylasttaken)--);
	return TRUE;
}
*/
int cvar_shutdown(void){
	int i = 0;
	if(cvar_list){
		for(i = 0; i <= cvar_arraylasttaken; i++){
			if(!cvar_list[i]->myid)continue;
			//TODO
//			_CONCAT(NAME, _unload)(&_CONCAT(NAME, _list)[i]);
//			_CONCAT(NAME, _remove)(_CONCAT(NAME, _list)[i].myid);
		}
		free(cvar_list);
	}
	cvar_list = 0;
	cvar_ok = 0;
	cvar_count = 0;
	cvar_roll = 1;
	cvar_arraysize = 0;
	cvar_arrayfirstopen = 0;
	cvar_arraylasttaken = -1;
	return i;
}
//aka cvar_addRINT
int cvar_register(cvar_t * inst){
	if(!inst || !inst->name) return FALSE;
	cvar_count++;
	cvar_roll++;
	if(cvar_roll > 65535) cvar_roll = 1;//no possible way for IDs to be 0 now
	for(; cvar_arrayfirstopen < cvar_arraysize && cvar_list[cvar_arrayfirstopen]->myid; cvar_arrayfirstopen++);
	if(cvar_arrayfirstopen == cvar_arraysize){
		cvar_arraysize++;
		cvar_list = realloc(cvar_list, cvar_arraysize * sizeof(cvar_t*));
	}
	cvar_list[cvar_arrayfirstopen] = inst;
	int returnid = (cvar_roll << 16) | cvar_arrayfirstopen;
	cvar_list[cvar_arrayfirstopen]->myid = returnid;

	hash_addToTable(inst->name, returnid, cvar_hashtable);
	if(cvar_arraylasttaken < cvar_arrayfirstopen) cvar_arraylasttaken = cvar_arrayfirstopen;

	inst->type |= CVAR_REGISTERED;

	if(inst->defaultstring){
		cvar_pset(inst, inst->defaultstring);
	}

	return returnid;
}//dont need rpoint because you already got rpoint lol

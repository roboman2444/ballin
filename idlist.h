//todo make this better and whatnot
#ifndef IDLISTHEADER
#define IDLISTHEADER


typedef struct idlist_s {
	int *list;
	unsigned int count;
} idlist_t;


#define _CONCET(x,y) x ## y
#define _CONCAT(x,y) _CONCET(x, y) //need that crazy redirection

#define NAME_LIST		_CONCAT(NAME, _list)
#define NAME_COUNT		_CONCAT(NAME, _count)
#define NAME_ROLL		_CONCAT(NAME, _roll)
#define NAME_ARRAYSIZE		_CONCAT(NAME, _arraysize)
#define NAME_ARRAYLASTTAKEN	_CONCAT(NAME, _arraylasttaken)
#define NAME_OK			_CONCAT(NAME, _ok)
#define NAME_HASHTABLE		_CONCAT(NAME, _hashtable)


#define IDLIST_HEADER(NAME, TYPE, TYPELIST)\
\
TYPE *	NAME_LIST;\
int	NAME_COUNT;\
int	NAME_ARRAYSIZE;\
int	NAME_ARRAYLASTTAKEN;\
int	NAME_OK;\
\
TYPE *	_CONCAT(NAME, _findByNameRPOINT)(const char * name);\
int	_CONCAT(NAME, _findByNameRINT)(const char * name);\
TYPELIST	_CONCAT(NAME, _findAllByNameRPOINT)(const char * name);\
idlist_t	_CONCAT(NAME, _findAllByNameRINT)(const char * name);\
TYPE *	_CONCAT(NAME, _returnById)(const int id);\
TYPE *	_CONCAT(NAME, _addRPOINT)(TYPE inst);\
int	_CONCAT(NAME, _addRINT)(TYPE inst);\
void	_CONCAT(NAME, _pruneList)(void);\
int 	_CONCAT(NAME, _delete)(const int id);\

#define IDLIST_INTERNAL(NAME, TYPE, TYPELIST)\
\
TYPE *	NAME_LIST =0;\
int	NAME_COUNT =0;\
int	NAME_ROLL =0;\
int	NAME_ARRAYSIZE =0;\
int	NAME_ARRAYFIRSTOPEN = 0;\
int	NAME_ARRAYLASTTAKEN = -1;\
int	NAME_OK =0;\
hashbucket_t	NAME_HASHTABLE[MAXHASHBUCKETS];\



#define IDLIST_INIT(NAME, TYPE, TYPELIST)\
memset(NAME_HASHTABLE, 0, MAXHASHBUCKETS * sizeof(hashbucket_t));\
if(NAME_LIST)free(NAME_LIST);\
NAME_LIST 	=0;\
NAME_COUNT 	=0;\
NAME_ROLL	=1;/*start it off at 1 for now, so i can test a typeless method*/\
NAME_ARRAYSIZE	=0;\
NAME_ARRAYLASTTAKEN	=-1;\


#define IDLIST_CODE(NAME, TYPE, TYPELIST)\
\
TYPE * _CONCAT(NAME, _findByNameRPOINT)(const char * name){\
	return _CONCAT(NAME, _returnById)(hash_findByNameRINT(name, NAME_HASHTABLE));\
}\
\
int _CONCAT(NAME, _findByNameRINT)(const char * name){\
	return hash_findByNameRINT(name, NAME_HASHTABLE);\
}\
\
TYPELIST _CONCAT(NAME, _findAllByNameRPOINT)(const char * name){\
	TYPELIST ret;\
	int hash = hash_get(name);\
	hashbucket_t * hb = &NAME_HASHTABLE[hash];\
	if(!hb->name) return ret;\
	for(; hb; hb = hb->next){\
		if(string_testEqual(hb->name, name)){\
		ret.count++;\
		ret.list = realloc(ret.list, ret.count * sizeof(TYPE *));\
		ret.list[ret.count-1] = _CONCAT(NAME, _returnById)(hb->id);\
		}\
	}\
	return ret;\
}\
\
idlist_t _CONCAT(NAME, _findAllByNameRINT)(const char * name){\
	idlist_t ret;\
	int hash = hash_get(name);\
	hashbucket_t * hb = &NAME_HASHTABLE[hash];\
	if(!hb->name) return ret;\
	for(; hb; hb = hb->next){\
		if(string_testEqual(hb->name, name)){\
		ret.count++;\
		ret.list = realloc(ret.list, ret.count * sizeof(int));\
		ret.list[ret.count-1] = hb->id;\
		}\
	}\
	return ret;\
}\
\
TYPE * _CONCAT(NAME, _returnById)(const int id){\
        int index = (id & 0xFFFF);\
        TYPE * ret = &NAME_LIST[index];\
        if(!ret->type) return FALSE;\
        if(ret->myid == id) return ret;\
        return FALSE;\
}\
\
void _CONCAT(NAME, _pruneList)(void){\
	if(NAME_ARRAYSIZE == NAME_ARRAYLASTTAKEN+1) return;\
	NAME_ARRAYSIZE = NAME_ARRAYLASTTAKEN+1;\
	NAME_LIST = realloc(NAME_LIST, NAME_ARRAYSIZE * sizeof(TYPE));\
}\
\
int _CONCAT(NAME, _delete)(const int id){\
	int index = (id & 0xFFFF);\
	TYPE * ret = &NAME_LIST[index];\
	if(ret->myid != id) return FALSE;\
	if(!ret->name) return FALSE;\
	NAME_COUNT--;\
	hash_deleteFromTable(ret->name, id, NAME_HASHTABLE);\
	free(ret->name);\
	memset(ret, 0, sizeof(TYPE));/*todo just test if setting type/id to 0 is good enough*/\
	if(index < NAME_ARRAYFIRSTOPEN) NAME_ARRAYFIRSTOPEN = index;\
	for(; NAME_ARRAYLASTTAKEN > 0 && !NAME_LIST[NAME_ARRAYLASTTAKEN].type; NAME_ARRAYLASTTAKEN--);\
	return TRUE;\
}\
\
int _CONCAT(NAME, _addRINT)(TYPE inst){\
	NAME_COUNT++;\
	NAME_ROLL++;\
	for(; NAME_ARRAYFIRSTOPEN < NAME_ARRAYSIZE && NAME_LIST[NAME_ARRAYFIRSTOPEN].type; NAME_ARRAYFIRSTOPEN++);\
	if(NAME_ARRAYFIRSTOPEN == NAME_ARRAYSIZE){\
		NAME_ARRAYSIZE++;\
		NAME_LIST = realloc(NAME_LIST, NAME_ARRAYSIZE * sizeof(TYPE));\
	}\
	NAME_LIST[NAME_ARRAYFIRSTOPEN] = inst;\
	int returnid = (NAME_ROLL << 16) | NAME_ARRAYFIRSTOPEN;\
	NAME_LIST[NAME_ARRAYFIRSTOPEN].myid = returnid;\
\
	hash_addToTable(NAME_LIST[NAME_ARRAYFIRSTOPEN].name, returnid, NAME_HASHTABLE);\
	if(NAME_ARRAYLASTTAKEN < NAME_ARRAYFIRSTOPEN) NAME_ARRAYLASTTAKEN = NAME_ARRAYFIRSTOPEN;\
	return returnid;\
}\
\
TYPE * _CONCAT(NAME, _addRPOINT)(TYPE inst){\
	NAME_COUNT++;\
	NAME_ROLL++;\
	for(; NAME_ARRAYFIRSTOPEN < NAME_ARRAYSIZE && NAME_LIST[NAME_ARRAYFIRSTOPEN].type; NAME_ARRAYFIRSTOPEN++);\
	if(NAME_ARRAYFIRSTOPEN == NAME_ARRAYSIZE){\
		NAME_ARRAYSIZE++;\
		NAME_LIST = realloc(NAME_LIST, NAME_ARRAYSIZE * sizeof(TYPE));\
	}\
	NAME_LIST[NAME_ARRAYFIRSTOPEN] = inst;\
	int returnid = (NAME_ROLL << 16) | NAME_ARRAYFIRSTOPEN;\
	NAME_LIST[NAME_ARRAYFIRSTOPEN].myid = returnid;\
\
	hash_addToTable(NAME_LIST[NAME_ARRAYFIRSTOPEN].name, returnid, NAME_HASHTABLE);\
	if(NAME_ARRAYLASTTAKEN < NAME_ARRAYFIRSTOPEN) NAME_ARRAYLASTTAKEN = NAME_ARRAYFIRSTOPEN;\
	return &NAME_LIST[NAME_ARRAYFIRSTOPEN];\
}\




//todo get rid of type... an id of 0 means invalid
//todo add option for no hashtables/names

#endif

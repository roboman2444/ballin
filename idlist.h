//todo make this better and whatnot
#ifndef IDLISTHEADER
#define IDLISTHEADER


typedef struct idlist_s {
	int *list;
	unsigned int count;
} idlist_t;


#define _CONCET(x,y) x ## y
#define _CONCAT(x,y) _CONCET(x, y) //need that crazy redirection

//unused, since they break
#define NAME_LIST		_CONCAT(NAME, _list)
#define NAME_COUNT		_CONCAT(NAME, _count)
#define NAME_ROLL		_CONCAT(NAME, _roll)
#define NAME_ARRAYSIZE		_CONCAT(NAME, _arraysize)
#define NAME_ARRAYFIRSTOPEN	_CONCAT(NAME, _arrayfirstopen)
#define NAME_ARRAYLASTTAKEN	_CONCAT(NAME, _arraylasttaken)
#define NAME_OK			_CONCAT(NAME, _ok)
#define NAME_HASHTABLE		_CONCAT(NAME, _hashtable)


#define IDLIST_HEADER_NOHASH(NAME, TYPE)\
\
TYPE *	_CONCAT(NAME, _list);\
int	_CONCAT(NAME, _count);\
int	_CONCAT(NAME, _arraysize);\
int	_CONCAT(NAME, _arraylasttaken);\
int	_CONCAT(NAME, _ok);\
\
TYPE *	_CONCAT(NAME, _returnById)(const int id);\
TYPE *	_CONCAT(NAME, _addRPOINT)(TYPE inst);\
int	_CONCAT(NAME, _addRINT)(TYPE inst);\
void	_CONCAT(NAME, _pruneList)(void);\
int 	_CONCAT(NAME, _delete)(const int id);\

#define IDLIST_HEADER(NAME, TYPE, TYPELIST)\
\
IDLIST_HEADER_NOHASH(NAME, TYPE);\
TYPE *	_CONCAT(NAME, _findByNameRPOINT)(const char * name);\
int	_CONCAT(NAME, _findByNameRINT)(const char * name);\
TYPELIST	_CONCAT(NAME, _findAllByNameRPOINT)(const char * name);\
idlist_t	_CONCAT(NAME, _findAllByNameRINT)(const char * name);\

#define IDLIST_INTERNAL_NOHASH(NAME, TYPE)\
TYPE *	_CONCAT(NAME, _list) =0;\
int	_CONCAT(NAME, _count) =0;\
int	_CONCAT(NAME, _roll) =1;\
int	_CONCAT(NAME, _arraysize) =0;\
int	_CONCAT(NAME, _arrayfirstopen) = 0;\
int	_CONCAT(NAME, _arraylasttaken) = -1;\
int	_CONCAT(NAME, _ok) =0;\

#define IDLIST_INTERNAL(NAME, TYPE, TYPELIST)\
IDLIST_INTERNAL_NOHASH(NAME, TYPE)\
hashbucket_t	_CONCAT(NAME, _hashtable)[MAXHASHBUCKETS];\



#define IDLIST_INIT_NOHASH(NAME, TYPE)\
if(_CONCAT(NAME, _list))free(_CONCAT(NAME, _list));\
_CONCAT(NAME, _list) 	=0;\
_CONCAT(NAME, _count) 	=0;\
_CONCAT(NAME, _roll)	=1;/*start it off at 1 for now, so i can test a typeless method*/\
_CONCAT(NAME, _arraysize)	=0;\
_CONCAT(NAME, _arraylasttaken)	=-1;\

#define IDLIST_INIT(NAME, TYPE, TYPELIST)\
IDLIST_INIT_NOHASH(NAME, TYPE)\
memset(_CONCAT(NAME, _hashtable), 0, MAXHASHBUCKETS * sizeof(hashbucket_t));\


#define IDLIST_CODE(NAME, TYPE, TYPELIST)\
\
TYPE * _CONCAT(NAME, _findByNameRPOINT)(const char * name){\
	return _CONCAT(NAME, _returnById)(hash_findByNameRINT(name, _CONCAT(NAME, _hashtable)));\
}\
\
int _CONCAT(NAME, _findByNameRINT)(const char * name){\
	return hash_findByNameRINT(name, _CONCAT(NAME, _hashtable));\
}\
\
TYPELIST _CONCAT(NAME, _findAllByNameRPOINT)(const char * name){\
	TYPELIST ret;\
	int hash = hash_get(name);\
	hashbucket_t * hb = &_CONCAT(NAME, _hashtable)[hash];\
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
	hashbucket_t * hb = &_CONCAT(NAME, _hashtable)[hash];\
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
        TYPE * ret = &_CONCAT(NAME, _list)[index];\
        if(!ret->type) return FALSE;\
        if(ret->myid == id) return ret;\
        return FALSE;\
}\
\
void _CONCAT(NAME, _pruneList)(void){\
	if(_CONCAT(NAME, _arraysize) == _CONCAT(NAME, _arraylasttaken)+1) return;\
	_CONCAT(NAME, _arraysize) = _CONCAT(NAME, _arraylasttaken)+1;\
	_CONCAT(NAME, _list) = realloc(_CONCAT(NAME, _list), _CONCAT(NAME, _arraysize) * sizeof(TYPE));\
}\
\
int _CONCAT(NAME, _delete)(const int id){\
	int index = (id & 0xFFFF);\
	TYPE * ret = &_CONCAT(NAME, _list)[index];\
	if(ret->myid != id) return FALSE;\
	_CONCAT(NAME, _count)--;\
	if(ret->name){\
		hash_deleteFromTable(ret->name, id, _CONCAT(NAME, _hashtable));\
		free(ret->name);\
	}\
	memset(ret, 0, sizeof(TYPE));/*todo just test if setting type/id to 0 is good enough*/\
	if(index < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arrayfirstopen) = index;\
	for(; _CONCAT(NAME, _arraylasttaken) > 0 && !_CONCAT(NAME, _list)[_CONCAT(NAME, _arraylasttaken)].type; _CONCAT(NAME, _arraylasttaken)--);\
	return TRUE;\
}\
\
int _CONCAT(NAME, _addRINT)(TYPE inst){\
	_CONCAT(NAME, _count)++;\
	_CONCAT(NAME, _roll)++;\
	for(; _CONCAT(NAME, _arrayfirstopen) < _CONCAT(NAME, _arraysize) && _CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].type; _CONCAT(NAME, _arrayfirstopen)++);\
	if(_CONCAT(NAME, _arrayfirstopen) == _CONCAT(NAME, _arraysize)){\
		_CONCAT(NAME, _arraysize)++;\
		_CONCAT(NAME, _list) = realloc(_CONCAT(NAME, _list), _CONCAT(NAME, _arraysize) * sizeof(TYPE));\
	}\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)] = inst;\
	int returnid = (_CONCAT(NAME, _roll) << 16) | _CONCAT(NAME, _arrayfirstopen);\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].myid = returnid;\
\
	if(inst.name)hash_addToTable(inst.name, returnid, _CONCAT(NAME, _hashtable));\
	if(_CONCAT(NAME, _arraylasttaken) < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arraylasttaken) = _CONCAT(NAME, _arrayfirstopen);\
	return returnid;\
}\
\
TYPE * _CONCAT(NAME, _addRPOINT)(TYPE inst){\
	_CONCAT(NAME, _count)++;\
	_CONCAT(NAME, _roll)++;\
	for(; _CONCAT(NAME, _arrayfirstopen) < _CONCAT(NAME, _arraysize) && _CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].type; _CONCAT(NAME, _arrayfirstopen)++);\
	if(_CONCAT(NAME, _arrayfirstopen) == _CONCAT(NAME, _arraysize)){\
		_CONCAT(NAME, _arraysize)++;\
		_CONCAT(NAME, _list) = realloc(_CONCAT(NAME, _list), _CONCAT(NAME, _arraysize) * sizeof(TYPE));\
	}\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)] = inst;\
	int returnid = (_CONCAT(NAME, _roll) << 16) | _CONCAT(NAME, _arrayfirstopen);\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].myid = returnid;\
\
	if(inst.name)hash_addToTable(inst.name, returnid, _CONCAT(NAME, _hashtable));\
	if(_CONCAT(NAME, _arraylasttaken) < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arraylasttaken) = _CONCAT(NAME, _arrayfirstopen);\
	return &_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)];\
}\





// a little bloaty here, could cut down on the code re-use
#define IDLIST_CODE_NOHASH(NAME, TYPE)\
TYPE * _CONCAT(NAME, _returnById)(const int id){\
        int index = (id & 0xFFFF);\
        TYPE * ret = &_CONCAT(NAME, _list)[index];\
        if(!ret->type) return FALSE;\
        if(ret->myid == id) return ret;\
        return FALSE;\
}\
\
void _CONCAT(NAME, _pruneList)(void){\
	if(_CONCAT(NAME, _arraysize) == _CONCAT(NAME, _arraylasttaken)+1) return;\
	_CONCAT(NAME, _arraysize) = _CONCAT(NAME, _arraylasttaken)+1;\
	_CONCAT(NAME, _list) = realloc(_CONCAT(NAME, _list), _CONCAT(NAME, _arraysize) * sizeof(TYPE));\
}\
\
int _CONCAT(NAME, _delete)(const int id){\
	int index = (id & 0xFFFF);\
	TYPE * ret = &_CONCAT(NAME, _list)[index];\
	if(ret->myid != id) return FALSE;\
	_CONCAT(NAME, _count)--;\
	memset(ret, 0, sizeof(TYPE));/*todo just test if setting type/id to 0 is good enough*/\
	if(index < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arrayfirstopen) = index;\
	for(; _CONCAT(NAME, _arraylasttaken) > 0 && !_CONCAT(NAME, _list)[_CONCAT(NAME, _arraylasttaken)].type; _CONCAT(NAME, _arraylasttaken)--);\
	return TRUE;\
}\
\
int _CONCAT(NAME, _addRINT)(TYPE inst){\
	_CONCAT(NAME, _count)++;\
	_CONCAT(NAME, _roll)++;\
	for(; _CONCAT(NAME, _arrayfirstopen) < _CONCAT(NAME, _arraysize) && _CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].type; _CONCAT(NAME, _arrayfirstopen)++);\
	if(_CONCAT(NAME, _arrayfirstopen) == _CONCAT(NAME, _arraysize)){\
		_CONCAT(NAME, _arraysize)++;\
		_CONCAT(NAME, _list) = realloc(_CONCAT(NAME, _list), _CONCAT(NAME, _arraysize) * sizeof(TYPE));\
	}\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)] = inst;\
	int returnid = (_CONCAT(NAME, _roll) << 16) | _CONCAT(NAME, _arrayfirstopen);\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].myid = returnid;\
\
	if(_CONCAT(NAME, _arraylasttaken) < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arraylasttaken) = _CONCAT(NAME, _arrayfirstopen);\
	return returnid;\
}\
\
TYPE * _CONCAT(NAME, _addRPOINT)(TYPE inst){\
	_CONCAT(NAME, _count)++;\
	_CONCAT(NAME, _roll)++;\
	for(; _CONCAT(NAME, _arrayfirstopen) < _CONCAT(NAME, _arraysize) && _CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].type; _CONCAT(NAME, _arrayfirstopen)++);\
	if(_CONCAT(NAME, _arrayfirstopen) == _CONCAT(NAME, _arraysize)){\
		_CONCAT(NAME, _arraysize)++;\
		_CONCAT(NAME, _list) = realloc(_CONCAT(NAME, _list), _CONCAT(NAME, _arraysize) * sizeof(TYPE));\
	}\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)] = inst;\
	int returnid = (_CONCAT(NAME, _roll) << 16) | _CONCAT(NAME, _arrayfirstopen);\
	_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)].myid = returnid;\
\
	if(_CONCAT(NAME, _arraylasttaken) < _CONCAT(NAME, _arrayfirstopen)) _CONCAT(NAME, _arraylasttaken) = _CONCAT(NAME, _arrayfirstopen);\
	return &_CONCAT(NAME, _list)[_CONCAT(NAME, _arrayfirstopen)];\
}\





//todo get rid of type... an id of 0 means invalid

#endif
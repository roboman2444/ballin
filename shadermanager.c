#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include "globaldefs.h"
#include "vertexattribs.h"
#include "shadermanager.h"
#include "stringlib.h"	//for idlist

#include "filesys.h"	//for file manipulation


#include "hashtables.h"	//for idlist
IDLIST_INTERNAL(shader, shader_t, shaderlist_t);

int shader_init(void){
	IDLIST_INIT(shader, shader_t, shaderlist_t);
	return TRUE;
}
//returns ID of shader program (either added or already there)
int shader_register(char * name){
	//check if shader exists
	int ret = shader_findByNameRINT(name);
	if(ret) return ret;
	shader_t s = {0};
	s.type = 1;
	s.name = strdup(name);
	return shader_addRINT(s);
}


int shader_load(shader_t *s){
	//todo a lot here
	switch(s->type){
		case 1:
			if(!s->name){
				printf("SHADER/shader_load error shader doesnt have a name?\n");
				return 0;
			}
			//todo move all this parsing stuff to filesys or something idk
			//load shader support file
			file_t f = file_open(s->name, "r");
			char * line;
			for(line = file_getEntireLine(&f); line; free(line), line = file_getEntireLine(&f)){
				//find deliminator
				char * c;
				char * strend = strlen(line) + line;
				char * delim = strchr(line, ':');
				if(!delim) delim = strend;
				*delim = 0;
				char * varname = line;
				char * vardata = delim+1;

				//trim whitespace from start of varname
				while(varname < delim && ISWHITESPACE(*varname)) varname++;
				c = delim-1;
				//empty line
				if(varname >= delim) varname = 0;
				//trim whitespace from end of varname
				else while(c > varname && ISWHITESPACE(*c)) *c--=0; //todo make sure this is OK

				//trim whitespace from start of vardata
				while(vardata < strend && ISWHITESPACE(*vardata)) vardata++;
				c = strend-1;
				//empty line
				if(vardata >= strend) vardata = 0;
				//trim whitespace from end of varname
				else while(c > vardata && ISWHITESPACE(*c)) *c--=0; //todo make sure this is OK

				printf("Varname: \"%s\"\t\tVardata: \"%s\"\n", varname, vardata);
			}
			file_close(&f);


			s->type =1;
		case 2:
		default: return s->type;
	}
}


int shader_unload(shader_t *m){
//TODO
	return FALSE;
}

IDLIST_CODE(shader, shader_t, shaderlist_t);

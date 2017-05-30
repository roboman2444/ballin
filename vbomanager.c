#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include "globaldefs.h"
#include "vertexattribs.h" //has to come before vbos because of the maxattribs
#include "vbomanager.h"



IDLIST_INTERNAL_NOHASH(vbo, vbo_t);


int vbo_init(void){
	IDLIST_INIT_NOHASH(vbo, vbo_t);
	return TRUE;
}


int vbo_setup(vbo_t *vbo){
	if(!vbo) return 0;

	switch(vbo->type){
		case 1:
			//generate buffers
			glGenVertexArrays(1, &vbo->vaoid);
			glGenBuffers(1, &vbo->vertsid);
			glGenBuffers(1, &vbo->facesid);
			vbo->type = 2;
		case 2:
			//set up attribs
			glBindVertexArray(vbo->vaoid);		//will eventually put this into a state
			glBindBuffer(GL_ARRAY_BUFFER, vbo->vertsid);
			//calculate total stride
			vbo->totalstride = 0;
			int i;
			for(i = 0; i < MAXATTRIBS; i++) vbo->totalstride += vbo->stridewidth[i];
			GLuint tstridebytes = vbo->totalstride * sizeof(GL_FLOAT);

			size_t cstride = 0;
			for(i = 0; i < MAXATTRIBS; i++){
				GLuint swidth = vbo->stridewidth[i];
				if(!swidth) continue;
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, swidth, GL_FLOAT, GL_FALSE, tstridebytes, (void*)cstride);
				cstride += swidth * sizeof(GL_FLOAT);
			}
			vbo->type = 3;
			return 3;
		default:
			return vbo->type;
	}
}


IDLIST_CODE_NOHASH(vbo, vbo_t);

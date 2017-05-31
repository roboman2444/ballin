#include "globaldefs.h"
#include "vbomanager.h"
#include "modelmanager.h"
#include "stringlib.h"	//for idlist

#include "iqm.h"	//for iqm handling


#include "hashtables.h"	//for idlist
IDLIST_INTERNAL(model, model_t, modellist_t);

int model_init(void){
	IDLIST_INIT(model, model_t, modellist_t);
	return TRUE;
}
//returns ID of model (either added or already there)
int model_register(char * name){
	//check if model exists
	int ret = model_findByNameRINT(name);
	if(ret) return ret;
	model_t m;
	m.type = 1;
	m.name = strdup(name);
	return model_addRINT(m);
}



int loadModelIQM(model_t *m){
	//somewhat copied from the SDK
	FILE *f = fopen(m->name, "rb");
	if(!f) return FALSE;

	unsigned char * buf = NULL;
	struct iqmheader hdr;
	if(fread(&hdr, 1, sizeof(hdr), f) != sizeof(hdr) || memcmp(hdr.magic, IQM_MAGIC, sizeof(hdr.magic)))
	goto error; //spaghetti!
	if(hdr.version != IQM_VERSION)goto error;
	if(hdr.filesize > (16<<20)) goto error; // sanity check... don't load files bigger than 16 MB
	buf = malloc(hdr.filesize);
	if(fread(buf + sizeof(hdr), 1, hdr.filesize - sizeof(hdr), f) != hdr.filesize - sizeof(hdr))
		goto error;
//todo actually load
//	if(hdr.num_meshes > 0 && !loadiqmmeshes(m, hdr, buf)) goto error;
//todo handle joints
//	if(hdr.num_joints > 0 && !loadiqmjoints(m, hdr, buf)) goto error;
//Not handling poses here
//Not handling IQM bboxes here
	if(m->interleaveddata) free(m->interleaveddata);
	m->interleaveddata = 0;

	fclose(f);
	free(buf);
	return TRUE;

	error:
	if(m->interleaveddata) free(m->interleaveddata);
	m->interleaveddata = 0;
	printf("MODEL/loadModelIQM error while loading %s\n", m->name);
	free(buf);
	fclose(f);
	return FALSE;
}

int model_load(model_t *m){
	switch(m->type){
		case 1:
			if(!loadModelIQM(m)) return FALSE;
			m->type = 2;
		case 2:
			m->type = 3;
		case 3:
			return 3;
		default:
		return FALSE;
	}
	return FALSE;
}

int model_unload(model_t *m){
	return FALSE;
}

IDLIST_CODE(model, model_t, modellist_t);

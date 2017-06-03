#include "globaldefs.h"
#include "stringlib.h"
#include "filesys.h"





char * file_getEntireLine(file_t *f){
	char * line = 0;

	size_t lsize = 0;
	int lplace =0;
	char c;
	for(c = fgetc(f->f); c != EOF && c != '\n'; c=fgetc(f->f)){
		if(lplace >= lsize) lsize = (lplace + 32 -1) & ~(32-1);
		line = realloc(line, lsize);
		line[lplace++] = c;
	}
	//check to make sure we arent reading nothing
	if(c == EOF && !lplace) return 0;

	//tighten up the alloc, add a nully
	if(lsize != lplace+1){
		line = realloc(line, lplace+1);
	}
	line[lplace++] = 0;

	return line;
}





//untested. todo
char * file_getEntireDelim(file_t *f, char * delims){
	return 0;
}


file_t file_open(const char * name, const char * mode){
	//todo more
	file_t f;
	f.f = fopen(name, mode);
	return f;
}


int file_close(file_t * f){
	//TODO more
	//TODO set the f->f to 0 after?
	return fclose(f->f);
}



size_t file_read(void *ptr, size_t size, size_t nmemb, file_t *f){
	//TODO more
	return fread(ptr, size, nmemb, f->f);
}
size_t file_write(const void *ptr, size_t size, size_t nmemb, file_t *f){
	//TODO more
	return fwrite(ptr, size, nmemb, f->f);
}

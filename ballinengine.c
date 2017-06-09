//Sys includes

//local includes
#include "globaldefs.h"
#include "ballinengine.h"
//#include "glmanager.h"

#include "glfwmanager.h" //init
#include "glmanager.h" //init

#include "cvarmanager.h"


int shutitdown(){
	printf("Shutting down\n");
	gl_shutdown();
	glfw_shutdown();
	cvar_shutdown();
	//todo
	return FALSE;
}


int main(int argc, char *argv[]){
	cvar_init();
	if(!glfw_init(800, 600, 24,1)){printf("Unable to init glfw\n"); shutitdown();}
	if(!gl_init()){printf("Unable to init gl\n"); shutitdown();}
	shutitdown();
	return FALSE;
}

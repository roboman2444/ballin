//Sys includes

//local includes
#include "globaldefs.h"
#include "ballinengine.h"
//#include "glmanager.h"

#include "glfwmanager.h" //init
#include "glmanager.h" //init


int shutitdown(){
	//todo
	return FALSE;
}


int main(int argc, char *argv[]){
	if(!glfw_init(800, 600, 24,1)){printf("Unable to init glfw\n"); shutitdown();}
	if(!gl_init()){printf("Unable to init gl\n"); shutitdown();}
	return FALSE;
}

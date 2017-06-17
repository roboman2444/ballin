

in vec3 posattrib;
in vec2 tcattrib;

uniform mat4 mvp;

out vec3 pos;
out vec2 tc;
void main(){
	gl_Position = mvp * vec4(posattrib, 1.0);
	pos = posattrib;
	tc = tcattrib;
}

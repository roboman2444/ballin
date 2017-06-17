in vec3 pos;
in vec2 tc;
uniform vec2 texturescale;
uniform sampler2D texture0;

out vec4 fragData0;
void main(){
	fragData0 = texture(texture0, uniform1 * tc);
}

#version 330 core

in vec3 ourColor;
out vec4 fragmentColor;

void main(){
	fragmentColor=vec4( ourColor,1.f );
}

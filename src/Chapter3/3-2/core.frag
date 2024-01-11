#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
out vec4 fragmentColor;

uniform sampler2D ourTexture1;

void main(){
	fragmentColor=texture( ourTexture1,TexCoord );
}

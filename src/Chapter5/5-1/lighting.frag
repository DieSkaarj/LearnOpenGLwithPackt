#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
//	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main(){
	/* Ambient */
	vec3 ambient=light.ambient*vec3( texture( material.diffuse,TexCoords ) );

	/* Diffuse */
	vec3 norm=normalize( Normal );
//	vec3 lightDir=normalize( light.position-FragPos );
	vec3 lightDir=normalize( -light.direction );
	float diff=max( dot( norm,lightDir ), 0.f );
	vec3 diffuse=light.diffuse*diff*vec3( texture( material.diffuse,TexCoords ) );

	/* Specular */
	vec3 viewDir=normalize( viewPos-FragPos );
	vec3 reflectDir=reflect( -lightDir,norm );
	float spec=pow( max( dot( viewDir,reflectDir ),0.f ),material.shininess );
	vec3 specular=light.specular*spec*vec3( texture( material.specular,TexCoords ) );

	vec3 result=ambient+diffuse+specular;
	color=vec4( result,1.f );
}

#version 330 core

#define NUMBER_OF_POINT_LIGHTS 4

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[ NUMBER_OF_POINT_LIGHTS ];
uniform SpotLight spotLight;

vec3 CalcDirLight( DirLight light,vec3 normal,vec3 viewDir );
vec3 CalcPointLight( PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir );
vec3 CalcSpotLight( SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir );

void main(){
	/* Properties */
	vec3 norm=normalize( Normal );
	vec3 viewDir=normalize( viewPos-FragPos );

	vec3 result=CalcDirLight( dirLight,norm,viewDir );

	/* Point Lights */
	for( int i=0;i<NUMBER_OF_POINT_LIGHTS;++i )
		result+=CalcPointLight( pointLight[ i ],norm,FragPos,viewDir );

	/* Spot Light */
	result+=CalcSpotLight( spotLight,norm,FragPos,viewDir );

	color=vec4( result,1.f );
}

vec3 CalcDirLight( DirLight light,vec3 normal,vec3 viewDir ){
	vec3 lightDir=normalize( -light.direction );

	/* Diffuse Shading */
	float diff=max( dot( normal,lightDir ),0.f );

	/* Specular Shading */
	vec3 reflectDir=reflect( -lightDir,normal );
	float spec=pow( max( dot( viewDir,reflectDir ),0.f ),material.shininess );

	/* Combine */
	vec3 ambient=light.ambient*vec3( texture( material.diffuse,TexCoords ) );
	vec3 diffuse=light.diffuse*diff*vec3( texture( material.diffuse,TexCoords ) );
	vec3 specular=light.specular*spec*vec3( texture( material.specular,TexCoords ) );

	return ( ambient+diffuse+specular );
}

vec3 CalcPointLight( PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir ){
	vec3 lightDir=normalize( light.position-fragPos );

	/* Diffuse Shading */
	float diff=max( dot( normal,lightDir ),0.f );

	/* Specular Shading */
	vec3 reflectDir=reflect( -lightDir,normal );
	float spec=pow( max( dot( viewDir,reflectDir ),0.f ),material.shininess );

	/* Attenuation */
	float distance=length( light.position-fragPos );
	float attenuation=1.f/( light.constant+light.linear*distance+light.quadratic*( distance*distance ) );

	/* Combine */
	vec3 ambient=light.ambient*vec3( texture( material.diffuse,TexCoords ) );
	vec3 diffuse=light.diffuse*diff*vec3( texture( material.diffuse,TexCoords ) );
	vec3 specular=light.specular*spec*vec3( texture( material.specular,TexCoords ) );

	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;

	return ( ambient+diffuse+specular );
}

vec3 CalcSpotLight( SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir ){
	vec3 lightDir=normalize( light.position-fragPos );

	/* Diffuse Shading */
	float diff=max( dot( normal,lightDir ),0.f );

	/* Specular Shading */
	vec3 reflectDir=reflect( -lightDir,normal );
	float spec=pow( max( dot( viewDir,reflectDir ),0.f ),material.shininess );

	/* Attenuation */
	float distance=length( light.position-fragPos );
	float attenuation=1.f/( light.constant+light.linear*distance+light.quadratic*( distance*distance ) );

	/* Intensity */
	float theta=dot( lightDir,normalize( -light.direction ) );
	float epsilon=light.cutOff-light.outerCutOff;
	float intensity=clamp( ( theta-light.outerCutOff )/epsilon,0.f,1.f );

	/* Combine */
	vec3 ambient=light.ambient*vec3( texture( material.diffuse,TexCoords ) );
	vec3 diffuse=light.diffuse*diff*vec3( texture( material.diffuse,TexCoords ) );
	vec3 specular=light.specular*spec*vec3( texture( material.specular,TexCoords ) );

	ambient*=attenuation*intensity;
	diffuse*=attenuation*intensity;
	specular*=attenuation*intensity;

	return ( ambient+diffuse+specular );
}

#include "Shader.hxx"
#include <cstdio>
#include <limits.h>
#include <iterator>
#include <array>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hxx"
#include "Texture.hxx"

const std::string \
	shaderPath{ "./src/Chapter6/" },
	imagePath{ "./res/Chapter4/" },
	windowTitle{ "Learn OpenGL: Chapter 6" };

const GLuint WIDTH{ 800 },HEIGHT{ 600 };
int SCREEN_WIDTH,SCREEN_HEIGHT;

void KeyCallback( GLFWwindow *window,int key,int scancode,int action,int mode );
void ScrollCallback( GLFWwindow *window,double xOffset,double yOffset );
void MouseCallback( GLFWwindow *window,double xPos,double yPos );
void DoMovement();

Camera camera( glm::vec3( 0.f,0.f,3.f ) );
GLfloat lastX{ WIDTH*0.5f },lastY{ HEIGHT*0.5f };

bool keys[1024],
     firstMouse{ true };

GLfloat deltaTime{ .0f },lastFrame{ .0f };

glm::vec3 lightPos( 1.2f,1.f,2.f );

/*
 *
*Entrance
 *
 */

constexpr int NUMBER_OF_POINT_LIGHTS{ 4 };

int main( int argv,char* argc[] ){
	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR,3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR,3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE );
	glfwWindowHint( GLFW_RESIZABLE,GL_FALSE );

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,windowTitle.c_str(),nullptr,nullptr ) };

	if( nullptr==window ){
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );
	glfwGetFramebufferSize( window,&SCREEN_WIDTH,&SCREEN_HEIGHT );

	glfwSetKeyCallback( window,KeyCallback );
	glfwSetCursorPosCallback( window,MouseCallback );
	glfwSetScrollCallback( window,ScrollCallback );

	glfwSetInputMode( window,GLFW_CURSOR,GLFW_CURSOR_DISABLED );

	glewExperimental=GL_TRUE;

	if( GLEW_OK != glewInit() ){
		std::cerr << "Failed to initialise GLEW\n";
		return EXIT_FAILURE;
	}

	glViewport( 0,0,SCREEN_WIDTH,SCREEN_HEIGHT );

	glEnable( GL_DEPTH_TEST );

	Shader shader( std::string( shaderPath+"cube.vs" ).c_str(),std::string( shaderPath+"cube.frag" ).c_str() );
	Shader skyboxShader( std::string( shaderPath+"skybox.vs" ).c_str(),std::string( shaderPath+"skybox.frag" ).c_str() );

	/* Create vertex data */
	/* Perspective Projection Vectices */
	const int cubeTotalVertVecs{ 288 }; /* 8(dimensions+normals+tex-coords)*6(verts)*6(faces) */
	std::array< GLfloat,cubeTotalVertVecs > vertices{
	// 		Positions		Texture Co-ords
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,
		0.5f,	-0.5f,	-0.5f,		1.f,0.f,
		0.5f,	0.5f,	-0.5f,		1.f,1.f,
		0.5f,	0.5f,	-0.5f,		1.f,1.f,
		-0.5f,	0.5f,	-0.5f,		0.f,1.f,
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,
        
		-0.5f,	-0.5f,	0.5f,		0.f,0.f,
		0.5f,	-0.5f,	0.5f,		1.f,0.f,
		0.5f,	0.5f,	0.5f,		1.f,1.f,
		0.5f,	0.5f,	0.5f,		1.f,1.f,
		-0.5f,	0.5f,	0.5f,		0.f,1.f,
		-0.5f,	-0.5f,	0.5f,		0.f,0.f,
        
		-0.5f,	0.5f,	 0.5f,		1.f,0.f,
		-0.5f,	0.5f,	-0.5f,		1.f,1.f,
		-0.5f,	-0.5f,	-0.5f,		0.f,1.f,
		-0.5f,	-0.5f,	-0.5f,		0.f,1.f,
		-0.5f,	-0.5f,	 0.5f,		0.f,0.f,
		-0.5f,	0.5f,	 0.5f,		1.f,0.f,
        
		0.5f,	0.5f,	0.5f,		1.f,0.f,
		0.5f,	0.5f,	-0.5f,		1.f,1.f,
		0.5f,	-0.5f,	-0.5f,		0.f,1.f,
		0.5f,	-0.5f,	-0.5f,		0.f,1.f,
		0.5f,	-0.5f,	 0.5f,		0.f,0.f,
		0.5f,	0.5f,	0.5f,		1.f,0.f,
        
		-0.5f,	-0.5f,	-0.5f,		0.f,1.f,
		0.5f,	-0.5f,	-0.5f,		1.f,1.f,
		0.5f,	-0.5f,	0.5f,		1.f,0.f,
		0.5f,	-0.5f,	0.5f,		1.f,0.f,
		-0.5f,	-0.5f,	0.5f,		0.f,0.f,
		-0.5f,	-0.5f,	-0.5f,        	0.f,1.f,

		-0.5f,	0.5f,	-0.5f,		0.f,1.f,
		0.5f,	0.5f,	-0.5f,		1.f,1.f,
		0.5f,	0.5f,	0.5f,		1.f,0.f,
		0.5f,	0.5f,	0.5f,		1.f,0.f,
		-0.5f,	0.5f,	0.5f,		0.f,0.f,
		-0.5f,	0.5f,	-0.5f,		0.f,1.f
	};

	std::array< GLfloat,108 > skyboxVertices{
		-1.f,	1.f,	-1.f,
		-1.f,	-1.f,	-1.f,
		1.f,	-1.f,	-1.f,
		1.f,	-1.f,	-1.f,
		1.f,	1.f,	-1.f,
		-1.f,	1.f,	-1.f,

		-1.f,	-1.f,	1.f,
		-1.f,	-1.f,	-1.f,
		-1.f,	1.f,	-1.f,
		-1.f,	1.f,	-1.f,
		-1.f,	1.f,	1.f,
		-1.f,	-1.f,	1.f,

		1.f,	-1.f,	-1.f,
		1.f,	-1.f,	1.f,
		1.f,	1.f,	1.f,
		1.f,	1.f,	1.f,
		1.f,	1.f,	-1.f,
		1.f,	-1.f,	-1.f,

		-1.f,	-1.f,	1.f,
		-1.f,	1.f,	1.f,
		1.f,	1.f,	1.f,
		1.f,	1.f,	1.f,
		1.f,	-1.f,	1.f,
		-1.f,	-1.f,	1.f,

		-1.f,	1.f,	-1.f,
		1.f,	1.f,	-1.f,
		1.f,	1.f,	1.f,
		1.f,	1.f,	1.f,
		-1.f,	1.f,	1.f,
		-1.f,	1.f,	-1.f,

		-1.f,	-1.f,	-1.f,
		-1.f,	-1.f,	1.f,
		1.f,	-1.f,	-1.f,
		1.f,	-1.f,	-1.f,
		-1.f,	-1.f,	1.f,
		1.f,	-1.f,	1.f
	};

	// Cube Object
	GLuint cubeVAO,cubeVBO;
	glGenVertexArrays( 1,&cubeVAO );
	glGenBuffers( 1,&cubeVBO );

	glBindBuffer( GL_ARRAY_BUFFER,cubeVBO );
	glBufferData( GL_ARRAY_BUFFER,vertices.size()*sizeof( glm::vec3 ),vertices.data(),GL_STATIC_DRAW );

	glBindVertexArray( cubeVAO );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,5*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 0 ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1,2,GL_FLOAT,GL_FALSE,5*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 3*sizeof( GLfloat ) ) );
	glBindVertexArray( 0 );

	// Skybox Object
	GLuint skyboxVAO,skyboxVBO;
	glGenVertexArrays( 1,&skyboxVAO );
	glGenBuffers( 1,&skyboxVBO );
	glBindVertexArray( skyboxVAO );
	glBindBuffer( GL_ARRAY_BUFFER,skyboxVBO );
	glBufferData( GL_ARRAY_BUFFER,skyboxVertices.size()*sizeof( glm::vec3 ),skyboxVertices.data(),GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,3*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 0 ) );
	glBindVertexArray( 0 );

	GLuint diffuseMap,specularMap,emissionMap;
	glGenTextures( 1,&diffuseMap );
	glGenTextures( 1,&specularMap );
	glGenTextures( 1,&emissionMap );

	GLuint cubeTexture{ TextureLoading::LoadTexture( const_cast< GLchar *>( std::string( imagePath+"container2.png" ).c_str() ) ) };
	glBindTexture( GL_TEXTURE_2D,0 );

	std::vector< const GLchar* > faces{
		"./res/skybox/right.tga",
		"./res/skybox/left.tga",
		"./res/skybox/top.tga",
		"./res/skybox/bottom.tga",
		"./res/skybox/back.tga",
		"./res/skybox/front.tga"
	};
	GLuint cubemapTexture{ TextureLoading::LoadCubemap( faces ) };

	glm::mat4 projection{ glm::perspective( camera.GetZoom(),static_cast< GLfloat >( SCREEN_WIDTH )/static_cast< GLfloat >( SCREEN_HEIGHT ),.1f,100.f ) };

	while( !glfwWindowShouldClose( window ) ){
		GLfloat currentFrame{ static_cast< GLfloat >( glfwGetTime() ) };
		deltaTime=currentFrame-lastFrame;
		lastFrame=currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor( .1f,.1f,.1f,1.f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::mat4 model{ 1.f },view{ camera.GetViewMatrix() };

		shader.Use();

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,cubeTexture );
		glUniform1i( glGetUniformLocation( shader._Program,"texture1" ),0 );

		GLint modelLoc{ glGetUniformLocation( shader._Program,"model" ) },
		      viewLoc{ glGetUniformLocation( shader._Program,"view" ) },
		      projLoc{ glGetUniformLocation( shader._Program,"projection" ) };

		glUniformMatrix4fv( viewLoc,1,GL_FALSE,glm::value_ptr( view ) );
		glUniformMatrix4fv( projLoc,1,GL_FALSE,glm::value_ptr( projection ) );

		glBindVertexArray( cubeVAO );

		glUniformMatrix4fv( modelLoc,1,GL_FALSE,glm::value_ptr( model ) );
		glDrawArrays( GL_TRIANGLES,0,36 );
		glBindVertexArray( 0 );

		glDepthFunc( GL_LEQUAL );
		skyboxShader.Use();
		view=glm::mat4( glm::mat3( camera.GetViewMatrix() ) );

		glUniformMatrix4fv( glGetUniformLocation( skyboxShader._Program,"view" ),1,GL_FALSE,glm::value_ptr( view ) );
		glUniformMatrix4fv( glGetUniformLocation( skyboxShader._Program,"projection" ),1,GL_FALSE,glm::value_ptr( projection ) );
		glBindVertexArray( skyboxVAO);
		glBindTexture( GL_TEXTURE_CUBE_MAP,cubemapTexture );
		glDrawArrays( GL_TRIANGLES,0,36 );
		glBindVertexArray( 0 );
		glDepthFunc( GL_LESS );

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1,&cubeVAO );
	glDeleteBuffers( 1,&cubeVBO );
	glDeleteVertexArrays( 1,&skyboxVAO );
	glDeleteBuffers( 1,&skyboxVBO );

	glfwTerminate();

	return EXIT_SUCCESS;
}

void DoMovement(){
	if( keys[ GLFW_KEY_W ] || keys[ GLFW_KEY_UP ] ) camera.ProcessKeyboard( FORWARD,deltaTime );
	if( keys[ GLFW_KEY_S ] || keys[ GLFW_KEY_DOWN ] ) camera.ProcessKeyboard( BACKWARD,deltaTime );
	if( keys[ GLFW_KEY_A ] || keys[ GLFW_KEY_LEFT ] ) camera.ProcessKeyboard( LEFT,deltaTime );
	if( keys[ GLFW_KEY_D ] || keys[ GLFW_KEY_RIGHT ] ) camera.ProcessKeyboard( RIGHT,deltaTime );
}

void KeyCallback( GLFWwindow *window,int key,int scancode,int action,int mode ){
	if( key==GLFW_KEY_ESCAPE && action==GLFW_PRESS ) glfwSetWindowShouldClose( window,GL_TRUE );

	if( key>=0 && key<1024 ){
		if( action==GLFW_PRESS ) keys[ key ]=true;
		else if( action == GLFW_RELEASE ) keys[ key ]=false;
	}
}

void MouseCallback( GLFWwindow *window,double xPos,double yPos ){
	if( firstMouse ){
		lastX=xPos;
		lastY=yPos;
		firstMouse=false;
	}

	GLfloat xOffset{  static_cast< GLfloat >( xPos-static_cast< double >( lastX ) ) },
		yOffset{ static_cast< GLfloat >( static_cast< double >( lastY )-yPos ) }; // inverted Y

	lastX=xPos; lastY=yPos;

	camera.ProcessMouseMovement( xOffset,yOffset );
}

void ScrollCallback( GLFWwindow *window,double xOffset,double yOffset ){
	camera.ProcessMouseScroll( yOffset );
}

#include "Shader.hxx"
#include <cstdio>
#include <iterator>
#include <vector>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Chapter3/3-3/Camera.h"

const std::string \
	shaderPath{ "./src/Chapter3/3-2/" },
      	imagePath{ "./res/Chapter3/image.jpg" };

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

/*
 *
*Entrance
 *
 */

int main( int argv,char* argc[] ){
	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR,	3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR,	3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE );
	glfwWindowHint( GLFW_RESIZABLE,GL_FALSE );

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL: Chapter 3-3",nullptr,nullptr ) };

	if( nullptr == window ){
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

	glewExperimental = GL_TRUE;

	if( GLEW_OK != glewInit() ){
		std::cerr << "Failed to initialise GLEW\n";
		return EXIT_FAILURE;
	}

	glViewport( 0,0,SCREEN_WIDTH,SCREEN_HEIGHT );

	glEnable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	Shader ourShader( std::string( shaderPath+"core.vs" ).c_str(),std::string( shaderPath+"core.frag" ).c_str() );

	/*
	*Create vertex data
	 */

	// Perspective Projection Vectices
	std::vector< GLfloat > vertices{
	// 		Positions		Texture Co-ordinates
		-0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,
        
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	1.0f,
		-0.5f,	0.5f,	0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
        
		-0.5f,	0.5f,	 0.5f,		1.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		-0.5f,	-0.5f,	 0.5f,		0.0f,	0.0f,
		-0.5f,	0.5f,	 0.5f,		1.0f,	0.0f,
        
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	-0.5f,	 0.5f,		0.0f,	0.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
        
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,
        
		-0.5f,	0.5f,	-0.5f,		0.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,		1.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,		0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,		0.0f,	1.0f
	};

	std::vector< glm::vec3 > cubePositions{
		glm::vec3( .0f,.0f,.0f ),
		glm::vec3( 2.f,5.f,-15.f ),
		glm::vec3( -1.5f,-2.2f,-2.5f ),
		glm::vec3( -3.8f,-2.f,-12.3f ),
		glm::vec3( 2.4f,-.4f,-3.5f ),
		glm::vec3( -1.7f,3.f,-7.5f ),
		glm::vec3( 1.3f,-2.f,-2.5f ),
		glm::vec3( 1.5f,2.f,-2.5f ),
		glm::vec3( 1.5f,.2f,-1.5f ),
		glm::vec3( -1.3f,1.f,-1.5f )
	};

	GLuint VBO,VAO;
	glGenVertexArrays( 1,&VAO );
	glGenBuffers( 1,&VBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBufferData( GL_ARRAY_BUFFER,vertices.size()*sizeof( glm::vec3 ),vertices.data(),GL_STATIC_DRAW );

	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,5*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 0 ) );
	glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 2,2,GL_FLOAT,GL_FALSE,5*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 3*sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 2 );

	glBindVertexArray( 0 );

	GLuint texture;
	int texW,texH;

	glGenTextures( 1,&texture );
	glBindTexture( GL_TEXTURE_2D,texture );

	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );

	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );

	unsigned char *image{ SOIL_load_image( imagePath.c_str(),&texW,&texH,0,SOIL_LOAD_RGBA ) };

	glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,texW,texH,0,GL_RGBA,GL_UNSIGNED_BYTE,image );
	glGenerateMipmap( GL_TEXTURE_2D );
	SOIL_free_image_data( image );
	glBindTexture( GL_TEXTURE_2D,0 );

	while( !glfwWindowShouldClose( window ) ){
		GLfloat currentFrame{ static_cast< GLfloat >( glfwGetTime() ) };
		deltaTime=currentFrame-lastFrame;
		lastFrame=currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor( .3f,.2f,.3f,1.f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		ourShader.Use();

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,texture );
		glUniform1i( glGetUniformLocation( ourShader._Program,"ourTexture" ),0 );

		glm::mat4 projection{ 1.f };
		projection =glm::perspective( camera.GetZoom(),static_cast< GLfloat >( SCREEN_WIDTH/SCREEN_HEIGHT ),.1f,1000.f );

		glm::mat4 view{ 1.f };
		view=camera.GetViewMatrix();
		
		GLint modelLoc{ glGetUniformLocation( ourShader._Program,"model") };
		GLint viewLoc{ glGetUniformLocation( ourShader._Program,"view") };
		GLint projectionLoc{ glGetUniformLocation( ourShader._Program,"projection") };

		glUniformMatrix4fv( viewLoc,1,GL_FALSE,glm::value_ptr( view ) );
		glUniformMatrix4fv( projectionLoc,1,GL_FALSE,glm::value_ptr( projection ) );

		glBindVertexArray( VAO );

		int cubeAngle{};
		for( auto cubePosition : cubePositions ){
			glm::mat4 model{ 1.f };
			model=glm::translate( model,cubePosition );
			GLfloat angle{ 20.f*( cubeAngle++ ) };
			model = glm::rotate( model,angle,glm::vec3( 1.f,.3f,.5f ) );
			glUniformMatrix4fv( modelLoc,1,GL_FALSE,glm::value_ptr( model ) );

			glDrawArrays( GL_TRIANGLES,0,36 );
		}

		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1,&VAO );
	glDeleteBuffers( 1,&VBO );

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

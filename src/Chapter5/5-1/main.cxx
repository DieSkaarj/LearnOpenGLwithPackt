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

const std::string \
	shaderPath{ "./src/Chapter5/5-1/" },
	imagePath{ "./res/Chapter4/" };

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
GLfloat lightAng( 0.f );

/*
 *
*Entrance
 *
 */

int main( int argv,char* argc[] ){
	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR,3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR,3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE );
	glfwWindowHint( GLFW_RESIZABLE,GL_FALSE );

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL: Chapter 5-1",nullptr,nullptr ) };

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

	Shader lightingShader( std::string( shaderPath+"lighting.vs" ).c_str(),std::string( shaderPath+"lighting.frag" ).c_str() );
	//Shader lampShader( std::string( shaderPath+"lamp.vs" ).c_str(),std::string( shaderPath+"lamp.frag" ).c_str() );

	/*
	*Create vertex data
	 */

	// Perspective Projection Vectices
	const int cubeTotalVertVecs{ 288 }; /* 8(dimensions+normals+tex-coords)*6(verts)*6(faces) */
	std::array< GLfloat,cubeTotalVertVecs > vertices{
	// 		Positions		Normals			Texture Co-ords
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,0.f,
		0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		-0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,0.f,
        
		-0.5f,	-0.5f,	0.5f,		0.f,0.f,-1.f,		0.f,0.f,
		0.5f,	-0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		-0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		-0.5f,	-0.5f,	0.5f,		0.f,0.f,-1.f,		0.f,0.f,
        
		-0.5f,	0.5f,	 0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		-0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		-0.5f,	-0.5f,	 0.5f,		0.f,0.f,-1.f,		0.f,0.f,
		-0.5f,	0.5f,	 0.5f,		0.f,0.f,-1.f,		1.f,0.f,
        
		0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		0.5f,	-0.5f,	 0.5f,		0.f,0.f,-1.f,		0.f,0.f,
		0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
        
		-0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		0.5f,	-0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		0.5f,	-0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		0.5f,	-0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		-0.5f,	-0.5f,	0.5f,		0.f,0.f,-1.f,		0.f,0.f,
		-0.5f,	-0.5f,	-0.5f,        	0.f,0.f,-1.f,		0.f,1.f,

		-0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f,
		0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		1.f,1.f,
		0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		1.f,0.f,
		-0.5f,	0.5f,	0.5f,		0.f,0.f,-1.f,		0.f,0.f,
		-0.5f,	0.5f,	-0.5f,		0.f,0.f,-1.f,		0.f,1.f
	};

	std::array< glm::vec3,10 > cubePositions{
		glm::vec3( 0.f,0.f,0.f ),
		glm::vec3( 2.f,5.f,-15.f ),
		glm::vec3( -1.5f,-2.2f,-2.5f ),
		glm::vec3( -3.8f,-2.f,-12.3f ),
		glm::vec3( 2.4f,-.4f,-3.5f ),
		glm::vec3( -1.7f,3.f,-7.5f ),
		glm::vec3( 1.3f,-2.f,-2.5f ),
		glm::vec3( 1.5f,2.f,-2.5f ),
		glm::vec3( 1.5f,0.2f,-1.5f ),
		glm::vec3( -1.3f,1.f,-1.5f ),
	};

	GLuint VBO,containerVAO/*,lightVAO*/;
	glGenVertexArrays( 1,&containerVAO );
	glGenBuffers( 1,&VBO );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBufferData( GL_ARRAY_BUFFER,vertices.size()*sizeof( glm::vec3 ),vertices.data(),GL_STATIC_DRAW );

	glBindVertexArray( containerVAO );
	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,8*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 0 ) );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 1,3,GL_FLOAT,GL_FALSE,8*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 3*sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 2,2,GL_FLOAT,GL_FALSE,8*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 6*sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 2 );
	glBindVertexArray( 0 );
/*
	glGenVertexArrays( 1,&lightVAO );
	glBindVertexArray( lightVAO );
	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,8*sizeof( GLfloat ),reinterpret_cast< GLvoid* >( 0 ) );
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
*/
	GLuint diffuseMap,specularMap;
	glGenTextures( 1,&diffuseMap );
	glGenTextures( 1,&specularMap );

	int textureWidth,textureHeight;
	unsigned char *image{ SOIL_load_image( std::string( imagePath+"container2.png" ).c_str(),&textureWidth,&textureHeight,0,SOIL_LOAD_RGB ) };
	glBindTexture( GL_TEXTURE_2D,diffuseMap );
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGB,textureWidth,textureHeight,0,GL_RGB,GL_UNSIGNED_BYTE,image );
	glGenerateMipmap( GL_TEXTURE_2D );
	SOIL_free_image_data( image );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST );

	image=SOIL_load_image( std::string( imagePath+"container2_specular.png" ).c_str(),&textureWidth,&textureHeight,0,SOIL_LOAD_RGB );
	glBindTexture( GL_TEXTURE_2D,specularMap );
	glTexImage2D( GL_TEXTURE_2D,0,GL_RGB,textureWidth,textureHeight,0,GL_RGB,GL_UNSIGNED_BYTE,image );
	glGenerateMipmap( GL_TEXTURE_2D );
	SOIL_free_image_data( image );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST_MIPMAP_NEAREST );

	glBindTexture( GL_TEXTURE_2D,0 );

	lightingShader.Use();
	glUniform1i( glGetUniformLocation( lightingShader._Program,"material.diffuse" ),0 );
	glUniform1i( glGetUniformLocation( lightingShader._Program,"material.specular" ),1 );

	glm::mat4 projection{ glm::perspective( camera.GetZoom(),static_cast< GLfloat >( SCREEN_WIDTH/SCREEN_HEIGHT ),.1f,100.f ) };

	while( !glfwWindowShouldClose( window ) ){
	//	lightPos.x -=0.00001f;
	//	lightPos.z -=0.00001f;

		GLfloat currentFrame{ static_cast< GLfloat >( glfwGetTime() ) };
		deltaTime=currentFrame-lastFrame;
		lastFrame=currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor( .2f,.1f,.2f,1.f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		lightingShader.Use();

		// Book describes using unsigned however glGetUni... returns signed, and there's no reason to cast it to unsigned.
		GLint	/*lightPosLoc{ glGetUniformLocation( lightingShader._Program,"light.position" ) },*/
			lightDirLoc{ glGetUniformLocation( lightingShader._Program,"viewPos" ) },
			viewPosLoc{ glGetUniformLocation( lightingShader._Program,"viewPos" ) },
			modelLoc{ glGetUniformLocation( lightingShader._Program,"model" ) },
			viewLoc{ glGetUniformLocation( lightingShader._Program,"view" ) },
			projLoc{ glGetUniformLocation( lightingShader._Program,"projection" ) };

		//glUniform3f( lightPosLoc,lightPos.x,lightPos.y,lightPos.z );
		glUniform3f( lightDirLoc,-.2f,-1.f,-.3f );
		glUniform3f( viewPosLoc,camera.GetPosition().x,camera.GetPosition().y,camera.GetPosition().z );

		glUniform3f( glGetUniformLocation( lightingShader._Program,"light.ambient" ),.2f,.2f,.2f );
		glUniform3f( glGetUniformLocation( lightingShader._Program,"light.diffuse" ),.5f,.5f,.5f );
		glUniform3f( glGetUniformLocation( lightingShader._Program,"light.specular" ),1.f,1.f,1.f );

		glUniform1f( glGetUniformLocation( lightingShader._Program,"material.shininess" ),32.f );

		glm::mat4 view{ camera.GetViewMatrix() };

		glUniformMatrix4fv( viewLoc,1,GL_FALSE,glm::value_ptr( view ) );
		glUniformMatrix4fv( projLoc,1,GL_FALSE,glm::value_ptr( projection ) );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,diffuseMap );

		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D,specularMap );

		glBindVertexArray( containerVAO );

		glm::mat4 model{ 1.f };

		int i{ INT_MAX };
		for( auto modelTransposition : cubePositions ){	
			// reset model matrix to 1.0 preventing null value
			model = glm::mat4( 1.f );
			model = glm::translate( model,modelTransposition );
			GLfloat angle{ 20.f*( ++i ) };
			model = glm::rotate( model,angle,glm::vec3( 1.f,.3f,.5f ) );
			//model = glm::scale( model,glm::vec3( .2f ) );

			glUniformMatrix4fv( modelLoc,1,GL_FALSE,glm::value_ptr( model ) );

			glDrawArrays( GL_TRIANGLES,0,36 ); // 6 vertices per face, 6 faces on a cube; 6(verts)*6(faces)=36(verts-total)
		}

		glBindVertexArray( 0 );
/*
		lampShader.Use();

		modelLoc = glGetUniformLocation( lampShader._Program,"model" );
		viewLoc = glGetUniformLocation( lampShader._Program,"view" );
		projLoc = glGetUniformLocation( lampShader._Program,"projection" );

		glUniformMatrix4fv( viewLoc,1,GL_FALSE,glm::value_ptr( view ) );
		glUniformMatrix4fv( projLoc,1,GL_FALSE,glm::value_ptr( projection ) );

//		glBindVertexArray( lightVAO );
		glDrawArrays( GL_TRIANGLES,0,36 );
		glBindVertexArray( 0 );
*/
		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1,&containerVAO );
//	glDeleteVertexArrays( 1,&lightVAO );
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

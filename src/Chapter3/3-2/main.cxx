#include "Shader.hxx"
#include <cstdio>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define ORTHO
//#define PERSPECTIVE

const std::string \
	shaderPath{ "./src/Chapter3/3-2/" },
      	imagePath{ "./res/Chapter3/image.jpg" };

const GLuint WIDTH{ 800 },HEIGHT{ 600 };

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

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL: Chapter 3-1",nullptr,nullptr ) };

	int scrW,scrH;
	glfwGetFramebufferSize( window,&scrW,&scrH );

	if( nullptr == window ){
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;

	if( GLEW_OK != glewInit() ){
		std::cerr << "Failed to initialise GLEW\n";
		return EXIT_FAILURE;
	}

	glViewport( 0,0,scrW,scrH );

	glEnable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	Shader ourShader( std::string( shaderPath+"core.vs" ).c_str(),std::string( shaderPath+"core.frag" ).c_str() );

	/*
	*Create vertex data
	 */

	#ifdef ORTHO
	// Orthographic Projection Vectices
	GLfloat vertices[]{
	//			 Positions				Texture Co-ordinates 
		-0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	0.0f,
		0.5f*500,	-0.5f*500,	-0.5f*500,		1.0f,	0.0f,
		0.5f*500,	0.5f*500,	-0.5f*500,		1.0f,	1.0f,
		0.5f*500,	0.5f*500,	-0.5f*500,		1.0f,	1.0f,
		-0.5f*500,	0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		-0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	0.0f,
        
		-0.5f*500,	-0.5f*500,	0.5f*500,		0.0f,	0.0f,
		0.5f*500,	-0.5f*500,	0.5f*500,		1.0f,	0.0f,
		0.5f*500,	0.5f*500,	0.5f*500,		1.0f,	1.0f,
		0.5f*500,	0.5f*500,	0.5f*500,		1.0f,	1.0f,
		-0.5f*500,	0.5f*500,	0.5f*500,		0.0f,	1.0f,
		-0.5f*500,	-0.5f*500,	0.5f*500,		0.0f,	0.0f,
        
		-0.5f*500,	0.5f*500,	 0.5f*500,		1.0f,	0.0f,
		-0.5f*500,	0.5f*500,	-0.5f*500,		1.0f,	1.0f,
		-0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		-0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		-0.5f*500,	-0.5f*500,	 0.5f*500,		0.0f,	0.0f,
		-0.5f*500,	0.5f*500,	 0.5f*500,		1.0f,	0.0f,
        
		0.5f*500,	0.5f*500,	0.5f*500,		1.0f,	0.0f,
		0.5f*500,	0.5f*500,	-0.5f*500,		1.0f,	1.0f,
		0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		0.5f*500,	-0.5f*500,	 0.5f*500,		0.0f,	0.0f,
		0.5f*500,	0.5f*500,	0.5f*500,		1.0f,	0.0f,
        
		-0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		0.5f*500,	-0.5f*500,	-0.5f*500,		1.0f,	1.0f,
		0.5f*500,	-0.5f*500,	0.5f*500,		1.0f,	0.0f,
		0.5f*500,	-0.5f*500,	0.5f*500,		1.0f,	0.0f,
		-0.5f*500,	-0.5f*500,	0.5f*500,		0.0f,	0.0f,
		-0.5f*500,	-0.5f*500,	-0.5f*500,		0.0f,	1.0f,
        
		-0.5f*500,	0.5f*500,	-0.5f*500,		0.0f,	1.0f,
		0.5f*500,	0.5f*500,	-0.5f*500,		1.0f,	1.0f,
		0.5f*500,	0.5f*500,	0.5f*500,		1.0f,	0.0f,
		0.5f*500,	0.5f*500,	0.5f*500,		1.0f,	0.0f,
		-0.5f*500,	0.5f*500,	0.5f*500,		0.0f,	0.0f,
		-0.5f*500,	0.5f*500,	-0.5f*500,		0.0f,	1.0f
	};
	#endif

	#ifdef PERSPECTIVE
	// Perspective Projection Vectices
	GLfloat vertices[]{
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
	#endif

	GLuint VBO,VAO;
	glGenVertexArrays( 1,&VAO );
	glGenBuffers( 1,&VBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBufferData( GL_ARRAY_BUFFER,sizeof( vertices ),vertices,GL_STATIC_DRAW );

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

	glm::mat4 projection{ 1.f };

	#ifdef PERSPECTIVE
	float aspect{ static_cast< float >( scrW )/static_cast< float >( scrH ) };
	projection=glm::perspective( 45.0f,static_cast< GLfloat >( aspect ),0.1f,100.f );
	#endif

	#ifdef ORTHO
	projection=glm::ortho( 0.f,static_cast< GLfloat >( scrW ),0.f,static_cast< GLfloat >( scrH ),0.1f,1000.f );
	#endif

	while( !glfwWindowShouldClose( window ) ){
		glfwPollEvents();

		glClearColor( .3f,.2f,.3f,1.f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,texture );
		glUniform1i( glGetUniformLocation( ourShader._Program,"ourTexture" ),0 );

		ourShader.Use();

		glm::mat4 model{ 1.f };
		glm::mat4 view{ 1.f };
		
		#ifdef PERSPECTIVE
		model = glm::rotate( model,static_cast< GLfloat >( glfwGetTime() )*1.f,glm::vec3( 0.5f,1.f,0.f ) );
		view = glm::translate( view,glm::vec3( 0.f,0.f,-3.f ) );
		#endif

		#ifdef ORTHO	
		model = glm::rotate( model,0.5f,glm::vec3( 1.0f,0.f,0.f ) );
		view = glm::translate( view,glm::vec3( scrW*.5f,scrH*.5f,-700.f ) );
		#endif

		GLint modelLoc{ glGetUniformLocation( ourShader._Program,"model") };
		GLint viewLoc{ glGetUniformLocation( ourShader._Program,"view") };
		GLint projectionLoc{ glGetUniformLocation( ourShader._Program,"projection") };

		glUniformMatrix4fv( modelLoc,1,GL_FALSE,glm::value_ptr( model ) );
		glUniformMatrix4fv( viewLoc,1,GL_FALSE,glm::value_ptr( view ) );
		glUniformMatrix4fv( projectionLoc,1,GL_FALSE,glm::value_ptr( projection ) );

		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES,0,36 );
		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1,&VAO );
	glDeleteBuffers( 1,&VBO );

	glfwTerminate();

	return EXIT_SUCCESS;
}


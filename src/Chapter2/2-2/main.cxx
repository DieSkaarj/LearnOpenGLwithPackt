#include "Shader.hxx"
#include <cstdio>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const std::string shaderPath{ "./src/Chapter2/2-2/" };

const GLuint WIDTH{ 800 },HEIGHT{ 600 };

/*
 *
 * Entrance
 *
 */

int main( int argv,char* argc[] ){

	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

	glfwWindowHint( GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE );
	glfwWindowHint( GLFW_RESIZABLE,GL_FALSE );

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL: Chapter 2-2",nullptr,nullptr ) };

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

	/*
	 * Create vertex data
	 */

	Shader ourShader( std::string( shaderPath+"core.vs" ).c_str(),std::string( shaderPath+"core.frag" ).c_str() );

	GLfloat vertices[]{
		/* Positions */		/* Colors */
		-0.5f,	-0.5f,	0.0f,	1.0f,	1.0f,	0.0f,	/* Bottom Right */
		0.5f,	-0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	/* Bottom Left */
		0.0f,	0.5f,	0.0f,	1.0f,	0.0f,	1.0f	/* Top */
	};

	GLuint VBO,VAO;
	glGenVertexArrays( 1,&VAO );
	glGenBuffers( 1,&VBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBufferData( GL_ARRAY_BUFFER,sizeof( vertices ),vertices,GL_STATIC_DRAW );

	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,6*sizeof( GL_FLOAT ),reinterpret_cast<GLvoid*>( 0 ) );
	glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 1,3,GL_FLOAT,GL_FALSE,6*sizeof( GL_FLOAT ),reinterpret_cast<GLvoid*>( 3*sizeof( GL_FLOAT ) ) );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );

	while( !glfwWindowShouldClose( window ) ){
		glfwPollEvents();
		glClearColor( .3f,.2f,.3f,1.f );
		glClear( GL_COLOR_BUFFER_BIT );

		ourShader.Use( );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES,0,3 );
		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1,&VAO );
	glDeleteBuffers( 1,&VBO );

	glfwTerminate();

	return EXIT_SUCCESS;
}


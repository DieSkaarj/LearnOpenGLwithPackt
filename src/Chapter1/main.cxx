#include <iostream>
#include <cstdio>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLint WIDTH{ 800 },HEIGHT{ 600 };

int main( int argv,char* argc[] ){

	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

	glfwWindowHint( GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE );
	glfwWindowHint( GLFW_RESIZABLE,GL_FALSE );

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL",nullptr,nullptr ) };

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

	while( !glfwWindowShouldClose( window ) ){
		glfwPollEvents();
		glClearColor( .2f,.3f,.3f,1.f );
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( window );
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}


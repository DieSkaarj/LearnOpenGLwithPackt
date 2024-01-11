#include <iostream>
#include <cstdio>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLuint WIDTH{ 800 },HEIGHT{ 600 };

/*
 *
 * Shaders
 *
 */

const GLchar *vertexShaderSource{
	"#version 330 core\n"
	"layout (location=0) in vec3 position;\n"
	"void main(){\n"
	"\tgl_Position=vec4( position.x,position.y,position.z,1.f );\n"
	"}\0"
};

const GLchar *fragmentShaderSource{
	"#version 330 core\n"
	"out vec4 fragmentColor;\n"
	"void main(){\n"
	"\tfragmentColor=vec4( 1.0f,1.0f,0.2f,1.0f );\n"
	"}\n\0"
};

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

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL: Chapter 2-1",nullptr,nullptr ) };

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
	 * Vertex Shader
	 */

	GLuint vertexShader{ glCreateShader( GL_VERTEX_SHADER ) };
	glShaderSource( vertexShader,1,&vertexShaderSource,nullptr );
	glCompileShader( vertexShader );

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv( vertexShader,GL_COMPILE_STATUS,&success );

	if( !success ){
		glGetShaderInfoLog( vertexShader,512,nullptr,infoLog );
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	/*
	 * Fragment Shader
	 */

	GLuint fragmentShader{ glCreateShader( GL_FRAGMENT_SHADER ) };
	glShaderSource( fragmentShader,1,&fragmentShaderSource,nullptr );
	glCompileShader( fragmentShader );

	glGetShaderiv( fragmentShader,GL_COMPILE_STATUS,&success );

	if( !success ){
		glGetShaderInfoLog( fragmentShader,512,nullptr,infoLog );
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	/*
	 * Link Shaders
	 */

	GLuint shaderProgram{ glCreateProgram( ) };
	glAttachShader( shaderProgram,vertexShader );
	glAttachShader( shaderProgram,fragmentShader );
	glLinkProgram( shaderProgram );

	glGetProgramiv( shaderProgram,GL_LINK_STATUS,&success );

	if( !success ){
		glGetProgramInfoLog( shaderProgram,512,nullptr,infoLog );
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	/*
	 * Create vertex data
	 */

	GLfloat vertices[]{
		-0.5f,	-0.5f,	0.0f,	/* Left */
		0.5f,	-0.5f,	0.0f,	/* Right */
		0.0f,	0.5f,	0.0f	/* Top */
	};

	GLuint VBO,VAO;
	glGenVertexArrays( 1,&VAO );
	glGenBuffers( 1,&VBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBufferData( GL_ARRAY_BUFFER,sizeof( vertices ),vertices,GL_STATIC_DRAW );

	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,3*sizeof( GL_FLOAT ),static_cast<GLvoid*>( 0 ) );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBindVertexArray( 0 );

	while( !glfwWindowShouldClose( window ) ){
		glfwPollEvents();
		glClearColor( .3f,.2f,.3f,1.f );
		glClear( GL_COLOR_BUFFER_BIT );

		glUseProgram( shaderProgram );
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


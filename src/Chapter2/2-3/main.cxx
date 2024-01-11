#include "Shader.hxx"
#include <cstdio>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>

const std::string \
		shaderPath{ "./src/Chapter2/2-3/" },
      		imagePath{ "./res/Chapter2/image.jpg" };

const GLuint WIDTH{ 800 },HEIGHT{ 600 };

/*
 *
 * Entrance
 *
 */

int main( int argv,char* argc[] ){

	glfwInit();

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

	glfwWindowHint( GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE );
	glfwWindowHint( GLFW_RESIZABLE,GL_FALSE );

	GLFWwindow *window{ glfwCreateWindow( WIDTH,HEIGHT,"Learn OpenGL: Chapter 2-3",nullptr,nullptr ) };

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
	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	Shader ourShader( std::string( shaderPath+"core.vs" ).c_str(),std::string( shaderPath+"core.frag" ).c_str() );

	/*
	 * Create vertex data
	 */

	GLfloat vertices[]{
		/* Positions */			/* Colors */			/* Texture Coords */
		0.5f,	0.5f,	0.0f,		1.0f,	1.0f,	1.0f,		1.0f,	1.0f,	/* Top Right */
		0.5f,	-0.5f,	0.0f,		0.75f,	0.5f,	0.25f,		1.0f,	0.0f,	/* Bottom Right */
		-0.5f,	-0.5f,	0.0f,		0.25f,	0.75f,	0.5f,		0.0f,	0.0f,	/* Bottom Left */
		-0.5f,	0.5f,	0.0f,		1.0f,	1.0f,	0.0f,		0.0f,	1.0f	/* Top Left */
	};

	GLuint indices[]{
		0,1,3,
		1,2,3
	};

	GLuint VBO,VAO,EBO;
	glGenVertexArrays( 1,&VAO );
	glGenBuffers( 1,&VBO );
	glGenBuffers( 1,&EBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER,VBO );
	glBufferData( GL_ARRAY_BUFFER,sizeof( vertices ),vertices,GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,sizeof( indices ),indices,GL_STATIC_DRAW );

	glVertexAttribPointer( 0,3,GL_FLOAT,GL_FALSE,8*sizeof( GL_FLOAT ),reinterpret_cast< GLvoid* >( 0 ) );
	glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 1,3,GL_FLOAT,GL_FALSE,8*sizeof( GL_FLOAT ),reinterpret_cast< GLvoid* >( 3*sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );

	glVertexAttribPointer( 2,2,GL_FLOAT,GL_FALSE,8*sizeof( GL_FLOAT ),reinterpret_cast< GLvoid* >( 6*sizeof( GLfloat ) ) );
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
		glfwPollEvents();
		glClearColor( .3f,.2f,.3f,1.f );
		glClear( GL_COLOR_BUFFER_BIT );

		ourShader.Use( );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,texture );
		glUniform1i( glGetUniformLocation( ourShader._Program,"ourTexture" ),0 );

		glBindVertexArray( VAO );
		glDrawElements( GL_TRIANGLES,6,GL_UNSIGNED_INT,0 );
		glBindVertexArray( 0 );
/*
		glDrawArrays( GL_TRIANGLES,0,3 );
		glBindVertexArray( 0 );
*/
		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1,&VAO );
	glDeleteBuffers( 1,&VBO );
	glDeleteBuffers( 1,&EBO );

	glfwTerminate();

	return EXIT_SUCCESS;
}


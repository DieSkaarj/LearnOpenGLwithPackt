#ifndef _SHADER_H
#define _SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

struct Shader{
	GLuint _Program;
	Shader( const GLchar *vertexPath,const GLchar *fragmentPath ){
		std::string vertexCode,fragmentCode;
		std::ifstream vShaderFile,fShaderFile;

		vShaderFile.exceptions( std::ifstream::badbit );
		vShaderFile.exceptions( std::ifstream::badbit );

		try{
			vShaderFile.open( vertexPath );
			fShaderFile.open( fragmentPath );
			std::stringstream vShaderStream,fShaderStream;

			vShaderStream << vShaderFile.rdbuf( );
			fShaderStream << fShaderFile.rdbuf( );

			vShaderFile.close( );
			fShaderFile.close( );

			vertexCode=vShaderStream.str( );
			fragmentCode=fShaderStream.str( );
		}
		catch( std::ifstream::failure &e ){
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
		}

		const GLchar *vShaderCode{ vertexCode.c_str( ) };
		const GLchar *fShaderCode{ fragmentCode.c_str( ) };

		GLuint vertex,fragment;
		GLint success;
		GLchar infoLog[512];

		/*
		 * Vertex Shader
		 */

		vertex=glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vertex,1,&vShaderCode,nullptr );
		glCompileShader( vertex );

		glGetShaderiv( vertex,GL_COMPILE_STATUS,&success );

		if( !success ){
			glGetShaderInfoLog( vertex,512,nullptr,infoLog );
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
		}

		/*
		 * Fragment Shader
		 */

		fragment=glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fragment,1,&fShaderCode,nullptr );
		glCompileShader( fragment );

		glGetShaderiv( fragment,GL_COMPILE_STATUS,&success );

		if( !success ){
			glGetShaderInfoLog( fragment,512,nullptr,infoLog );
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		}

		/*
		 * Link Shaders
		 */

		this->_Program=glCreateProgram( );
		glAttachShader( this->_Program,vertex );
		glAttachShader( this->_Program,fragment );
		glLinkProgram( this->_Program );

		glGetProgramiv( this->_Program,GL_LINK_STATUS,&success );

		if( !success ){
			glGetProgramInfoLog( this->_Program,512,nullptr,infoLog );
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
		}

		glDeleteShader( vertex );
		glDeleteShader( fragment );
	}

	void Use( ){
		glUseProgram( this->_Program );
	}
};

#endif//_SHADER_H

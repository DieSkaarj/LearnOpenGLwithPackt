//#pragma
#ifndef _CAMERA_H
#define _CAMERA_H

#include <vector>

#define GLEW_STATIC

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement{
	FORWARD,BACKWARD,LEFT,RIGHT
};

const GLfloat	YAW{ -90.f },
      		PITCH{ 0.0f },
		SPEED{ 6.f },
		SENSITIVITY{ 0.25f },
		ZOOM{ 45.f };

class Camera{
	public:
	
	Camera( glm::vec3 position=glm::vec3( 0.f,0.f,0.f ),
		glm::vec3 up=glm::vec3( 0.f,1.f,0.f ),
		GLfloat yaw=YAW,GLfloat pitch=PITCH ): \

		front( glm::vec3( 0.f,0.f,-1.f ) ),movementSpeed( SPEED ),
		mouseSensitivity( SENSITIVITY ),zoom( ZOOM ){
			this->position=position;
			this->worldUp=up;
			this->yaw=yaw;
			this->pitch=pitch;
			this->updateCameraVectors();
	}

	Camera( GLfloat posX,GLfloat posY,GLfloat posZ,
		GLfloat upX,GLfloat upY,GLfloat upZ,
		GLfloat yaw,GLfloat pitch ): \

		front( glm::vec3( 0.f,0.f,-1.f ) ),
		movementSpeed( SPEED ),
		mouseSensitivity( SENSITIVITY ),
		zoom( ZOOM ){
			this->position=glm::vec3( posX,posY,posZ );
			this->worldUp=glm::vec3( upX,upY,upZ );
			this->yaw=yaw;
			this->pitch=pitch;
			this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix(){
		return glm::lookAt( this->position,this->position+this->front,this->up );
	}

	void ProcessKeyboard( Camera_Movement direction,GLfloat deltaTime ){
		GLfloat velocity{ this->movementSpeed*deltaTime };

		if( FORWARD==direction ) this->position+=this->front*velocity;
		if( BACKWARD==direction ) this->position-=this->front*velocity;
		if( LEFT==direction ) this->position-=this->right*velocity;
		if( RIGHT==direction ) this->position+=this->right*velocity;
	}

	void ProcessMouseMovement( GLfloat xOffset,GLfloat yOffset,GLboolean constrainPitch=true  ){
		xOffset*=this->mouseSensitivity;
		yOffset*=this->mouseSensitivity;

		this->yaw+=xOffset;
		this->pitch+=yOffset;

		if( constrainPitch ){
			if( this->pitch > 89.f ) this->pitch=89.f;
			if( this->pitch < -89.f ) this->pitch=-89.f;
		}

		this->updateCameraVectors();
	}

	void ProcessMouseScroll( GLfloat yOffset ){
		if( this->zoom >= 1.f && this->zoom <= 45.f ) this->zoom -= yOffset;
		if( this->zoom <= 1.f ) this->zoom=1.f;
		if( this->zoom >= 45.f ) this->zoom=45.f;
	}

	GLfloat GetZoom(){
		return this->zoom;
	}

	glm::vec3 GetPosition(){
		return this->position;
	}

	private:

	glm::vec3 position,front,up,right,worldUp;
	GLfloat yaw,pitch;
	GLfloat movementSpeed,mouseSensitivity,zoom;

	void updateCameraVectors(){
		glm::vec3 front;
		front.x=cos( glm::radians( this->yaw ) )*cos( glm::radians( this->pitch ) );
		front.y=sin( glm::radians( this->pitch ) );
		front.z=sin( glm::radians( this->yaw ) )*cos( glm::radians( this->pitch ) );

		this->front=glm::normalize( front );
		this->right=glm::normalize( glm::cross( this->front,this->worldUp ) );
		this->up=glm::normalize( glm::cross( this->right,this->front ) );
	}
};

#endif//_CAMERA_H

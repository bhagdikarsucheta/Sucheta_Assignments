#pragma once

#include<vector>

//#define GLEW_STATIC
#include <gl\glew.h>
#include <gl\gl.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\rotate_vector.hpp>

#define _USE_MATH_DEFINES	1
#include <math.h>


#define YAW  -90.0f
#define PITCH  0.0f
#define SPEED  10.0f
#define SENSITIVITY  0.05f
#define ZOOM  25.0f
#define ROLL  0.0f


enum ECameraMovement
{
	E_FORWARD = 0,
	E_BACKARD,
	E_LEFT,
	E_RIGHT,
	E_UP,
	E_DOWN
};

class Camera
{

private:

	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	GLfloat m_Yaw;
	GLfloat m_Pitch;
	GLfloat m_Roll;

	GLfloat m_MovementSpeed;
	GLfloat m_MouseSensitivity;
	GLfloat m_Zoom;

void UpdateCameraVectors();

public:
	Camera(
		glm::vec3 v3Position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 v3UP = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW,
		GLfloat pitch = PITCH,
		GLfloat Roll = ROLL
	) :
		m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_MovementSpeed(SPEED),
		m_MouseSensitivity(SENSITIVITY),
		m_Zoom(ZOOM)
	{
		this->m_Position = v3Position;
		this->m_WorldUp = v3UP;
		this->m_Yaw = yaw;
		this->m_Pitch = pitch;
		this->m_Roll = Roll;

		this->UpdateCameraVectors();
	}

	Camera(
		GLfloat posX, GLfloat posY, GLfloat posZ,
		GLfloat upX, GLfloat upY, GLfloat upZ,
		GLfloat yaw,
		GLfloat pitch,
		GLfloat Roll
	) :
		m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_MovementSpeed(SPEED),
		m_MouseSensitivity(SENSITIVITY),
		m_Zoom(ZOOM)
	{
		this->m_Position = glm::vec3(posX, posY, posZ);
		this->m_WorldUp = glm::vec3(upX, upY, upZ);
		this->m_Yaw = yaw;
		this->m_Pitch = pitch;
		this->m_Roll = Roll;

		this->UpdateCameraVectors();
	}


	glm::mat4 GetViewMatrix(void);
	GLfloat GetZoom(void);
	glm::vec3 GetCameraPosition(void);
	glm::vec3 GetCameraFront(void);
	glm::vec3 GetCameraUp(void);
	glm::vec3 GetCameraRight(void);


	void SetCameraPosition(glm::vec3 newPosition);
	void SetCameraFront(glm::vec3 newFront);

	//void InvertPitch();
	void InvertCamera();

	void ProcessKeyBoard(ECameraMovement direction, GLfloat deltaTime);
	void ProcessMouseMovements(GLfloat xOffset, GLfloat yOffset, GLboolean constarintPitch = true);
	void ProcessMouseScrool(GLfloat yOffset, GLfloat xOffset);


	void MoveForward(GLfloat Velocity, GLfloat DeltaTime);
	void MoveBackward(GLfloat Velocity, GLfloat DeltaTime);
	void MoveUpward(GLfloat Velocity, GLfloat DeltaTime);
	void MoveDownward(GLfloat Velocity, GLfloat DeltaTime);
	void StrafeRight(GLfloat Velocity, GLfloat DeltaTime);
	void StrafeLeft(GLfloat Velocity, GLfloat DeltaTime);


	//~Camera();

};


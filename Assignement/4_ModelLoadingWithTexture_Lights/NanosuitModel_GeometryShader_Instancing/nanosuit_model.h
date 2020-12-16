#pragma once
#include <Windows.h>
#include<stdio.h>
#include<time.h>
#include<GL/glew.h>
#include<gl/GL.h>
#include<glm/glm.hpp>
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"vmath.h"
#include"model.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

using namespace vmath;
//

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//Global variable declarations
FILE* gpFile = NULL;

HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;



bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

bool gbLight;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cameraSpeed = 0.3f;

__int64 currTime;
__int64 elapsedTime;
__int64 countsPerSec;



class nanosuit_model
{
	DWORD dwStyle;
	WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
	GLuint gVertexShaderObject;
	GLuint gGeometryShaderObject;
	GLuint gFragmentShaderObject;
	GLuint gShaderProgramObject;


	GLuint gVertexShaderObject_Two;
	GLuint gGeometryShaderObject_Two;
	GLuint gFragmentShaderObject_Two;
	GLuint gShaderProgramObject_Two;
	GLuint gTexture_sampler_uniform;

	GLuint gVao;
	GLuint gVbo;

	GLuint gVaoColor;
	GLuint gVboColor;
	GLuint gMVPUniform;
	GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;

	GLuint gNumberOfSegmentsUniform;
	GLuint gNumberOfStripsUniform;
	GLuint gLineColorUniform;
	unsigned int gNumberOfLineSegments;

	GLuint L_KeyPressed_uniform;
	GLuint B_KeyPressed_uniform;

	GLuint La_uniform;
	GLuint Ld_uniform;
	GLuint Ls_uniform;
	GLuint light_position_uniform;

	GLuint Ka_uniform;
	GLuint Kd_uniform;
	GLuint Ks_uniform;
	GLuint material_shininess_uniform;
	GLfloat lightAmbient[5] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat lightDiffuse[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightSpecular[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightPosition[5] = { 0.0f,1.0f,4.0f,1.0f };

	GLfloat material_ambient[5] = { 0.20f,0.20f,0.20f,1.0f };
	GLfloat material_diffuse[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat material_specular[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat material_shininess = 128.0f;

	mat4 gPerspectiveProjectionMatrix;
	Model* myModel = NULL;
	time_t curr_time;
	float currentFrame = (float)time(&curr_time);

	float radius = 12.0f;

public:
	void ToggleFullScreen(void);
	void uninitialize(void);
	void resize(int, int);
	void display(void);
	void initialize();

};
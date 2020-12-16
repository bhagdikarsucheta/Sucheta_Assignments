

#include<Windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>
#include"vmath.h"

//Prototype of WndProc() declared gloabally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;


bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

#define IDBITMAP_SMILEY 101

class MySmiley
{
	DWORD dwStyle;
	WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };


	enum
	{
		VDG_ATTRIBUTE_VERTEX = 0,
		VDG_ATTRIBUTE_COLOR,
		VDG_ATTRIBUTE_NORMAL,
		VDG_ATTRIBUTE_TEXTURE0,
	};

	GLuint gVertexShaderObject;
	GLuint gFragmentShaderObject;
	GLuint gShaderProgramObject;

	GLuint gVao_Smiley;
	GLuint gVbo_Smiley_position;
	GLuint gVbo_Smiley_texture;

	GLuint gTexture_sampler_uniform;

	GLuint gMVPUniform;
	GLuint gTextureSmiley;

	vmath::mat4 gPerspectiveProjectionMatrix;

public:

	int LoadGLTextures(GLuint*, TCHAR[]);

	void resize(int width, int height);
	void display();

	void initialize(void);
	void uninitialize(void);

	void ToggleFullScreen(void);


};











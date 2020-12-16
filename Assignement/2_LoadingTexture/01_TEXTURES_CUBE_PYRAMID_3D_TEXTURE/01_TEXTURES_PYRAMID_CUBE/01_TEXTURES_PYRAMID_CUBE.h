#include<Windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>

#include"vmath.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define IDBITMAP_KUNDALI 100
#define IDBITMAP_STONE 101


#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")

//Global variable declarations

using namespace vmath;
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;
//Prototype of WndProc() declared gloabally

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


class TexturePyramidCube
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

	GLuint gVao_pyramid;
	GLuint gVbo_pyramid_position;
	GLuint gVbo_pyramid_texture;

	GLuint gVao_Cube;
	GLuint gVbo_cube_position;
	GLuint gVbo_cube_texture;

	GLuint gMVPUniform;

	mat4 gPerspectiveProjectionMatrix;

	GLfloat gAnglePyramid = 0.0f;
	GLfloat gAngleCube = 0.0f;

	GLuint gTexture_sampler_uniform;
	GLuint gTexture_Kundali;
	GLuint gTexture_Stone;




public:
	void ToggleFullScreen(void);
	void initialize(void);
	int LoadGLTextures(GLuint* texture, TCHAR imageResourceId[]);
	void display();
	void resize(int width, int height);
	void uninitialize(void);
	void update();


};























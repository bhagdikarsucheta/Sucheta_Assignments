#pragma once
#include<windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>

#include"vmath.h"


using namespace vmath;

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define IDBITMAP_KUNDALI 100
#define IDBITMAP_STONE 101
#define IDBITMAP_MOON 102

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

//Prototype of WndProc() declared gloabally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;
bool gbLight;


class LightsSpherePerFragment
{
	int MAXSIZE = 16;
	int stack[16];
	int top = -1;
	mat4 insertModelMatrix[124];

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

	GLuint gNumElements;
	GLuint gNumVertices;


	GLuint sphere_Vao;
	GLuint sphere_bo_position;
	GLuint sphere_vbo_normal;
	GLuint sphere_vbo_texture;
	GLuint sphere_vbo_index;


	// Sphere Functions Start
	GLushort* gsphereElements;
	GLfloat* gsphereVerts;
	GLfloat* gsphereNorms;
	GLfloat* gsphereTexCoords;


	GLuint giNumElements = 0;
	GLuint giMaxElements = 0;
	GLuint giNumVertices = 0;
	GLuint gTexture_sampler_uniform;


	GLuint gVBOSphere = 0;
	GLuint gVBOSpherePosition = 0;
	GLuint gVBOSphereNormal = 0;
	GLuint gVBOSphereTexture = 0;
	GLuint gVBOSphereElement = 0;

	float sphere_vertices[1146];
	float sphere_normals[1146];
	float sphere_textures[764];
	unsigned short sphere_elements[2280];

	GLuint gVao_Sphere;
	GLuint gVbo_Sphere_positions;
	GLuint gVbo_Sphere_normal;
	GLuint gVbo_Sphere_element;

	GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;

	GLuint L_KeyPressed_uniform;

	GLuint La_uniform;
	GLuint Ld_uniform;
	GLuint Ls_uniform;
	GLuint light_position_uniform;

	GLuint Ka_uniform;
	GLuint Kd_uniform;
	GLuint Ks_uniform;
	GLuint material_shininess_uniform;

	mat4 gPerspectiveProjectionMatrix;

	GLfloat lightAmbient[5] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat lightDiffuse[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightSpecular[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightPosition[5] = { 0.0f,0.0f,0.0f,1.0f };

	GLfloat material_ambient[5] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat material_diffuse[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat material_specular[5] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat material_shininess = 128.0f;

	GLfloat gAngle = 0.0f;
	GLfloat gAngle2 = 0.0f;
	GLuint gTexture_Stone;
	GLuint gTexture_Earth;
	GLuint gTexture_Moon;

public:


	int LoadGLTextures(GLuint* texture, TCHAR imageResourceId[]);
	void initialize(void);
	void ToggleFullScreen(void);

	//Sphere making code
	void allocateSphere(GLint iNumIndices);
	void addTriangles(GLfloat** single_vertex, GLfloat** single_normal, GLfloat** single_texture);
	void makeSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);
	void prepareToDraw();
	void normalizeVector(GLfloat* gfVector);
	bool IsFoundIdentical(GLfloat val1, GLfloat val2, GLfloat diff);
	void drawSphere();
	GLuint getIndexCountSphere();
	GLuint getVertexCountSphere();
	void releaseSphereMemory(GLfloat** Block);
	void cleanupMeshDataSphere();

	void display();
	void resize(int width, int height);
	void uninitialize(void);

};
	
#include <Windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>


#include <glm\glm.hpp>
#include <glm\geometric.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>


#include"vmath.h"
#include"Camera.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define DELTA 0.00166666666666667f
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

using namespace vmath;

enum
{
	VDG_ATTRIBUTE_VERTEX = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

//Prototype of WndProc() declared Globally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//Global variable declarations
FILE *gpFile = NULL;

GLfloat gAnglePyramid = 0.0f;
GLfloat gAngleCube = 0.0f;

HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;


DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

GLuint gVertexShaderObject=0;
GLuint gFragmentShaderObject=0;
GLuint gShaderProgramObject = 0;

GLuint gVao_Pyramid;
GLuint gVbo_Position_Pyramid;
GLuint gVbo_Color_Pyramid;

GLuint gVao_Cube;
GLuint gVbo_Position_Cube;
GLuint gVbo_Color_Cube;

GLuint gModelMatrix;
GLuint gViewMatrix;
GLuint gProjectionMatrix;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
GLfloat g_fLastX = WIN_WIDTH / 2;
GLfloat g_fLastY = WIN_HEIGHT / 2;

GLfloat g_DeltaTime = 0.0f;
GLboolean g_bFirstMouse = true;
GLfloat g_fCurrentWidth;
GLfloat g_fCurrentHeight;

glm::mat4 gPerspectiveProjectionMatrix;

//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototype
	void display();
	void initialize();
	void uninitialize(void);
	void update();

	//variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("OpenGLPP");
	bool bDone = false;

	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Can not be Created\Exitting.."), TEXT("ERROR"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log file is Successfully Openend \n");
	}

	//code
	//initializing member of WNDCLASS
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;

	//Registering Class
	RegisterClassEx(&wndclass);

	//CreateWindow
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		TEXT("OpenGL Programmable PipeLine Using Native Windowing: First ortho Trianle Window Shree Ganeshaya Namaha"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ghwnd = hwnd;

	//intitialize()
	initialize();

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Message Loop
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//display();
			if (gbActiveWindow == true)
			{
				update();
				display();

				if (gbEscapeKeyIsPressed == true)
					bDone = true;
			}
		}
	}

	uninitialize();
	return((int)msg.wParam);

}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototypes
	//	void display();
	void resize(int, int);
	void ToggleFullScreen(void);
	void uninitialize(void);

	//code
	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbActiveWindow = true;
		else
			gbActiveWindow = false;
		break;
	case WM_PAINT:
		//		display();
		break;

	case WM_SIZE:
	{
		g_fCurrentWidth = LOWORD(lParam);
		g_fCurrentHeight = LOWORD(lParam);
		resize(LOWORD(lParam), HIWORD(lParam));
	}
	break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;

		case 0x41://if 'A' is pressed
			camera.ProcessKeyBoard(E_LEFT, DELTA);
			break;

		case 0x44://if 'D' is pressed
			camera.ProcessKeyBoard(E_RIGHT, DELTA);
			break;

		case 0x57:
			camera.ProcessKeyBoard(E_FORWARD, DELTA);
			break;

		case 0x53:
			camera.ProcessKeyBoard(E_BACKARD, DELTA);
			break;


		case 0x46:			//for 'f' or 'F'
			if (gbFullScreen == false)
			{
				ToggleFullScreen();
				gbFullScreen = true;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = false;
			}
			break;
		default:
			break;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		GLfloat xPos = LOWORD(lParam);
		GLfloat yPos = HIWORD(lParam);


		if (g_bFirstMouse)
		{
			g_fLastX = xPos;
			g_fLastY = yPos;

			g_bFirstMouse = false;
		}

		GLfloat xOffset = xPos - g_fLastX;
		GLfloat yOffset = g_fLastY - yPos;

		g_fLastX = g_fCurrentWidth / 2;
		g_fLastY = g_fCurrentHeight / 2;

		camera.ProcessMouseMovements(xOffset, yOffset);

	}
	break;

	case WM_MOUSEHWHEEL:
	{
		GLfloat xPos = LOWORD(lParam);
		GLfloat yPos = HIWORD(lParam);
		camera.ProcessMouseScrool(xPos, yPos);
	}
	break;
	
	case WM_LBUTTONDOWN:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;



	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}


void ToggleFullScreen(void)
{
	//variable declarations
	MONITORINFO mi;

	//code
	if (gbFullScreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}

		}
		ShowCursor(FALSE);
	}
	else
	{
		//code
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

void initialize()
{
	//function prototype
	void uninitialize(void);
	void resize(int, int);

	//variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//Initialization code
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 8;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//******** VERTEX SHADER ******
	//create shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to shader
	const GLchar *vertexShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec4 vColor;"	\
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \

		"out vec4 out_Color;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_projection_matrix * u_view_matrix*u_model_matrix*vPosition;" \
		"out_Color = vColor;" \
		"}";

	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;

	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log:%s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//**********FRAGMENT SHADER*********
	//create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec4 out_Color;" \
		"out vec4 FragColor;" \
		"void main (void)" \
		"{" \
		"FragColor = out_Color;" \
		"}";

	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iShaderCompiledStatus);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//********** SHADER PROGRAM *********
	//create
	gShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	//pre-link binding of shader program object with vertex position attribute
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");

	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_COLOR, "vColor");

	//Link Shader
	glLinkProgram(gShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program LinK Log %s\n");
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	//get MVP uniform locaion
	gModelMatrix = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	gViewMatrix = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	gProjectionMatrix = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	//**** vertices, colors, shader attribs, vbo, vao initializations ****//

	const GLfloat pyramidVertices[] =
	{
		//FRONT FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		//RIGHT FACE
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		//LEFT FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		//BACK FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f
	};

	const GLfloat colorVerticesPyramid[] =
	{
		//FRONT FACE
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,

		//RIGHT FACE
		1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,

		//BACK FACE
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,

		//LEFT FACE
		1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0
	};



	//Start of VAO Pyramid
	glGenVertexArrays(1, &gVao_Pyramid);
	glBindVertexArray(gVao_Pyramid);
	//start of VboPositio Pyramid 
	glGenBuffers(1, &gVbo_Position_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position_Pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER, 0);		
	//fininshing/completion of above gVboPosition buffer state

												//VBO For colors
	//start of vboColor Pyramid 
	glGenBuffers(1, &gVbo_Color_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Color_Pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorVerticesPyramid), colorVerticesPyramid, GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);	//fininshing/completion of above gVboColor buffer state

	glBindVertexArray(0);		//fininshing vaoTriangle

	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glBackFace(GL_CW);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gPerspectiveProjectionMatrix = glm::mat4(1.0);
	resize(WIN_WIDTH, WIN_HEIGHT);
	ToggleFullScreen();

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 m4PersPectiveProjectionMatrix = glm::perspective(camera.GetZoom(), (float)g_fCurrentWidth / (float)g_fCurrentHeight, 0.1f, 100.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::mat4 viewMatrix = glm::mat4(1.0);
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	glm::mat4 scaleMatrix = glm::mat4(1.0);
	

	//start using OpenGL program Object
	glUseProgram(gShaderProgramObject);

	modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(glm::mat4(1.0), gAnglePyramid, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = modelMatrix * rotationMatrix;

	glUniformMatrix4fv(gModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(gViewMatrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(gProjectionMatrix, 1, GL_FALSE, glm::value_ptr(gPerspectiveProjectionMatrix));

	glBindVertexArray(gVao_Pyramid);

	// ****  draw either by glDrawTriangles() or glDrawarrays() or glDrawElements()
	glDrawArrays(GL_TRIANGLES, 0, 12);			//(each with its x,y,z ) vertices in triangleVertices array

												//**** unbind vaoTriangle ****
	glBindVertexArray(0);
	SwapBuffers(ghdc);
	SetCursorPos((int)(g_fCurrentWidth / 2), (int)(g_fCurrentHeight / 2));
} 

void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gPerspectiveProjectionMatrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void uninitialize(void)
{
	//UNINITIALIZATION CODE
	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

	//destroy vao
	if (gVao_Pyramid)
	{
		glDeleteVertexArrays(1, &gVao_Pyramid);
		gVao_Pyramid = 0;
	}

	//destroy vbo
	if (gVbo_Position_Pyramid)
	{
		glDeleteVertexArrays(1, &gVbo_Position_Pyramid);
		gVbo_Position_Pyramid = 0;
	}

	if (gVbo_Color_Pyramid)
	{
		glDeleteVertexArrays(1, &gVbo_Color_Pyramid);
		gVbo_Color_Pyramid = 0;
	}

	if (gVao_Cube)
	{
		glDeleteVertexArrays(1, &gVao_Cube);
		gVao_Cube = 0;
	}

	if (gVbo_Position_Cube)
	{
		glDeleteVertexArrays(1, &gVbo_Position_Cube);
		gVbo_Position_Cube = 0;
	}

	if (gVbo_Color_Cube)
	{
		glDeleteVertexArrays(1, &gVbo_Color_Cube);
		gVbo_Color_Cube = 0;
	}

	//detach vertex shader from shader program object
	glDetachShader(gShaderProgramObject, gVertexShaderObject);

	//detach fragment shader from shader program object
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);

	//delete vertex shader object 
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;

	//delete fragment shader object
	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	//unlink shader program
	glUseProgram(0);

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;

}



void update()
{
	gAnglePyramid = gAnglePyramid + 0.1f;
	if (gAnglePyramid >= 360.0f)
		gAnglePyramid = 0.0f;

	/*gAngleCube = gAngleCube + 0.1;;
	if (gAngleCube >= 360.0f)
		gAngleCube = 0.0f;*/
}












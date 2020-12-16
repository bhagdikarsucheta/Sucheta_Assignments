#include"nanosuit_model.h"

nanosuit_model nanosuit;
int initial_time = (int)time(NULL), final_time, frame_count = 0;
//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{

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
		TEXT("OpenGL Programmable PipeLine Using Native Windowing: Geometry Shader White Triangles Shree Ganeshaya Namaha"),
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
	nanosuit.initialize();

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
				nanosuit.display();

				if (gbEscapeKeyIsPressed == true)
					bDone = true;
			}
		}
	}

	nanosuit.uninitialize();
	return((int)msg.wParam);

}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototypes
	//	void display();
	static bool gbIsLKeyPressed = false;

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
		nanosuit.resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{

	
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;

		case 'W':
		case 'w':
			cameraPos += cameraSpeed * cameraFront;

			break;
		case 'S':
		case 's':
			cameraPos -= cameraSpeed * cameraFront;
			break;

		case 'A':
		case 'a':
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;

		case 'D':
		case 'd':
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

			break;
		case 0x4c:		//for l or L key (Light)
			if (gbIsLKeyPressed == false)
			{
				gbLight = true;
				gbIsLKeyPressed = true;
			}
			else
			{
				gbLight = false;
				gbIsLKeyPressed = false;
			}
			break;

		case 0x46:			//for 'f' or 'F'
			if (gbFullScreen == false)
			{
				nanosuit.ToggleFullScreen();
				gbFullScreen = true;
			}
			else
			{
				nanosuit.ToggleFullScreen();
				gbFullScreen = false;
			}
			break;
		default:
			break;
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


void nanosuit_model::ToggleFullScreen(void)
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

void nanosuit_model::initialize()
{
	

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


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//******** VERTEX SHADER ******
	//create shader
	gVertexShaderObject_Two = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to shader
	const GLchar* vertexShaderSourceCodeTwo =
		"#version 430" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vColor;"\
		"in vec2 vTexture0_coord;" \
		"in vec3 vNormal;" \

		"out VertexData{"\
		"vec2 out_texture0_coord;"\
		"vec3 color;"\
		"vec3 transformed_normals;" \
		"vec3 light_direction;" \
		"vec3 viewer_vector;" \
		"}vs_out;"\

		"uniform mat4 u_projection_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_model_matrix;" \

		"uniform vec4 u_light_position;" \
		"uniform int u_lighting_enabled;" \

		"void main(void)" \
		"{" \
		"	if(u_lighting_enabled == 1)" \
		"	{" \
		"		vec4 eye_coordinates = u_view_matrix*u_model_matrix*vPosition;" \
		"		vs_out.transformed_normals = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
		"		vs_out.light_direction = vec3(u_light_position) - eye_coordinates.xyz;" \
		"		vs_out.viewer_vector = -eye_coordinates.xyz;" \
		"	}" \
		"gl_Position = u_view_matrix * u_model_matrix * vPosition;" \
		"vs_out.color = vColor;"\
		"vs_out.out_texture0_coord = vTexture0_coord;"\
		"}";

	glShaderSource(gVertexShaderObject_Two, 1, (const GLchar**)&vertexShaderSourceCodeTwo, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_Two);
	int iInfoLength = 0;
	int iShaderCompiledStatus = 0;
	char* szInfoLog = NULL;

	glGetShaderiv(gVertexShaderObject_Two, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject_Two, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject_Two, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log:%s\n", szInfoLog);
				free(szInfoLog);
				nanosuit.uninitialize();
				exit(0);
			}
		}
	}


	//*****************GEOMETRY SHADER************************
	gGeometryShaderObject_Two = glCreateShader(GL_GEOMETRY_SHADER);

	//provide code to shader 
	const GLchar* geometryShaderSourceTwo =
		"#version 430" \
		"\n" \
		"layout(triangles,invocations = 32)in;"\
		"layout(triangle_strip,max_vertices = 3)out;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_model_matrix;" \

		"in VertexData{"\
		"vec2 out_texture0_coord;"\
		"vec3 color;"\
		"vec3 transformed_normals;" \
		"vec3 light_direction;" \
		"vec3 viewer_vector;" \
		"}gs_in[];"\



		"out VertexData{"\
		"vec2 out_texture0_coord;"\
		"vec3 transformed_normals;" \
		"vec3 light_direction;" \
		"vec3 viewer_vector;" \
		"}vertexOut;"\


		"out vec3 fcolor;"\
		"float rand(vec2 co)"\
		"{"\
		"	return fract(sin(dot(co.xy ,vec2(8.9898,78.233))) * 43758.5453);"\
		"}"\
		"float randx = 70*rand(vec2(gl_InvocationID,0.0f));"\
		//"float randy =  rand(vec2(gl_InvocationID,0.0f));"\

		"float randy =  2.0* rand(vec2(gl_InvocationID,0.0f));"\
		"float randz = -80.0* rand(vec2(gl_InvocationID,0.0f));"\

		"float randy_second_vertex =  -8.0* rand(vec2(gl_InvocationID,0.0f));"\
		"float randx_plus = 90*rand(vec2(gl_InvocationID,0.0f));"\
		"float randz_minus_incr = -40.0* rand(vec2(gl_InvocationID,0.0f));"\

		"float randx_plus_third = 140*rand(vec2(gl_InvocationID,0.0f));"\
		"float randy_third_vertex =  -20.0* rand(vec2(gl_InvocationID,0.0f));"\
		"float randz_minus_incr_third = -20*rand(vec2(gl_InvocationID,0.0f));"\

		"void main(void)" \
		"{"\
		"	float movx = -50.0 * rand(vec2(gl_InvocationID,0.0));"	\
		"	float movy = 80.0 * rand(vec2(gl_InvocationID,gl_InvocationID));"	\

		"	mat4 u_mvp_matrix = u_projection_matrix * u_view_matrix * u_model_matrix;"	\

		"	gl_Position = u_projection_matrix * (gl_in[0].gl_Position + vec4(movx,0.0,movy,0.0f) ) ;"	\
		"	vertexOut.out_texture0_coord=gs_in[0].out_texture0_coord;"	\
		"	vertexOut.transformed_normals=gs_in[0].transformed_normals;"	\
		"	vertexOut.light_direction=gs_in[0].light_direction;"	\
		"	vertexOut.viewer_vector=gs_in[0].viewer_vector;"	\
		"	EmitVertex();"	\

		"	gl_Position = u_projection_matrix * (gl_in[1].gl_Position + vec4(movx,0.0f,movy,0.0f) ) ;"	\
		"	vertexOut.out_texture0_coord=gs_in[1].out_texture0_coord;"	\
		"	vertexOut.transformed_normals=gs_in[1].transformed_normals;"	\
		"	vertexOut.light_direction=gs_in[1].light_direction;"	\
		"	vertexOut.viewer_vector=gs_in[1].viewer_vector;"	\
		"	EmitVertex();"	\

		"	gl_Position = u_projection_matrix * (gl_in[2].gl_Position + vec4(movx,0.0f,movy,0.0f) ) ;"	\
		"	vertexOut.out_texture0_coord=gs_in[2].out_texture0_coord;"	\
		"	vertexOut.transformed_normals=gs_in[2].transformed_normals;"	\
		"	vertexOut.light_direction=gs_in[2].light_direction;"	\
		"	vertexOut.viewer_vector=gs_in[2].viewer_vector;"	\

		"	EmitVertex();"	\
		"	EndPrimitive();"	\

		"}";

	glShaderSource(gGeometryShaderObject_Two, 1, (const GLchar**)&geometryShaderSourceTwo, NULL);

	glCompileShader(gGeometryShaderObject_Two);
	iInfoLength = 0;
	iShaderCompiledStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(gGeometryShaderObject_Two, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gGeometryShaderObject_Two, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gGeometryShaderObject_Two, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Geometry Shader Compilation Log:%s\n", szInfoLog);
				free(szInfoLog);
				nanosuit.uninitialize();
				exit(0);
			}
		}
	}






	//////**** FRAGMENT SHADER ****
	gFragmentShaderObject_Two = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCodeTwo =
		"#version 430" \
		"\n" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Ls;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \
		"uniform float u_material_shininess;" \
		"uniform int u_lighting_enabled;" \

		"in VertexData{"\
		"vec2 out_texture0_coord;"\
		"vec3 transformed_normals;" \
		"vec3 light_direction;" \
		"vec3 viewer_vector;" \
		"}fragout;"\



		"uniform sampler2D u_texture0_sampler;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"vec4 FragLight;"\
		"vec4 texColor;" \
		"texColor = texture(u_texture0_sampler,fragout.out_texture0_coord);" \

		"if(u_lighting_enabled==1)"\
		"{" \
		"	vec3 normalized_transformed_normals=normalize(fragout.transformed_normals);" \
		"	vec3 normalized_light_direction=normalize(fragout.light_direction);" \
		"	vec3 normalized_viewer_vector=normalize(fragout.viewer_vector);" \
		"	vec3 ambient = u_La * u_Ka;" \
		"	float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
		"	vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
		"	vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normals);" \
		"	vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector,normalized_viewer_vector),0.0),u_material_shininess);" \
		"	phong_ads_color = ambient + diffuse + specular;" \
		"	FragLight = vec4(phong_ads_color,1.0);"\
		"	FragColor = texColor  * FragLight;" \
		"}"\
		"else"\
		"{"\
		"	FragColor = vec4(1,1,1,1);" \
		"}"\
		"}";

	glShaderSource(gFragmentShaderObject_Two, 1, (const GLchar**)&fragmentShaderSourceCodeTwo, NULL);

	//compile shader
	glCompileShader(gFragmentShaderObject_Two);
	glGetShaderiv(gFragmentShaderObject_Two, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iShaderCompiledStatus);
		if (iInfoLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject_Two, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				nanosuit.uninitialize();
				exit(0);
			}
		}
	}






	//********** SHADER PROGRAM *********
	//create
	gShaderProgramObject_Two = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObject_Two, gVertexShaderObject_Two);

	// attach geometry control shader to shader program
	glAttachShader(gShaderProgramObject_Two, gGeometryShaderObject_Two);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObject_Two, gFragmentShaderObject_Two);


	glBindAttribLocation(gShaderProgramObject_Two, VDG_ATTRIBUTE_VERTEX, "vPosition");
	glBindAttribLocation(gShaderProgramObject_Two, VDG_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_Two, VDG_ATTRIBUTE_TEXTURE0, "vTexture0_coord");

	// link shader
	glLinkProgram(gShaderProgramObject_Two);
	int iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject_Two, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject_Two, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject_Two, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				nanosuit.uninitialize();
				exit(0);
			}
		}
	}


	//get MVP uniform locaion
	gMVPUniform = glGetUniformLocation(gShaderProgramObject_Two, "u_mvp_matrix");
	model_matrix_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_model_matrix");
	view_matrix_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_view_matrix");
	projection_matrix_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_projection_matrix");
	gTexture_sampler_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_texture0_sampler");
	L_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_lighting_enabled");
	B_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject, "u_fragment_enabled");

	La_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_La");
	//diffuse color intensity of LIGHT
	Ld_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_Ld");
	//specular color intensity of LIGHT
	Ls_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_Ls");
	//position of light
	light_position_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_light_position");

	//ambient reflective color intensity of MATERIAL
	Ka_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_Ka");
	//diffuse reflective color intensity of MATERIAL
	Kd_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_Kd");

	//specular reflective color intensity of MATERIAL
	Ks_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_Ks");
	material_shininess_uniform = glGetUniformLocation(gShaderProgramObject_Two, "u_material_shininess");

	//myModel = new Model("C:/Users/Sucheta/Desktop/04_SHADERS_COLOR_TRIANGLE/nanosuit/nanosuit.obj");
	myModel = new Model("nanosuit/nanosuit.obj");
	//	myModel = new Model("C:/Users/Sucheta/Desktop/Tree_Geometry_Instancing/04_SHADERS_COLOR_TRIANGLE/ganesha.obj");

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gPerspectiveProjectionMatrix = mat4::identity();
	nanosuit.resize(WIN_WIDTH, WIN_HEIGHT);
	nanosuit.ToggleFullScreen();

}

void nanosuit_model::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//start using OpenGL program Object
	glUseProgram(gShaderProgramObject_Two);
	if (gbLight == true)
	{

		//set u_lighting_enabled uniform
		glUniform1i(L_KeyPressed_uniform, 1);

		//setting lights properties
		glUniform3fv(La_uniform, 1, lightAmbient);
		glUniform3fv(Ld_uniform, 1, lightDiffuse);
		glUniform3fv(Ls_uniform, 1, lightSpecular);
		glUniform4fv(light_position_uniform, 1, lightPosition);

		//setting material's properties
		glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);

	}
	else
	{
		glUniform1i(L_KeyPressed_uniform, 0);

	}
	//OpenL Drawing 
	mat4 modelMatrix = mat4::identity();
	glm::mat4 viewMatrix;

	////////////////////////////////////////////Camera movement on keys


	mat4 projectionMatrix = mat4::identity();
	mat4 rotationmatrix = mat4::identity();
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//	modelMatrix = translate(-18.0f, -4.8f, -15.0f);
	modelMatrix = translate(-1.0f, -2.8f, -35.0f);
	//	modelMatrix = vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f);

	//multipley the modelview and orthographic matrix to get modelviewprojection matrix
	projectionMatrix = gPerspectiveProjectionMatrix * modelMatrix;	//ORDER IS I
	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, projectionMatrix);
	glUniform1i(gTexture_sampler_uniform, 0);

	myModel->Draw(gShaderProgramObject);

	glUseProgram(0);

	SwapBuffers(ghdc);

}

void nanosuit_model::resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gPerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);
}

void nanosuit_model::uninitialize(void)
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
	if (gVao)
	{
		glDeleteVertexArrays(1, &gVao);
		gVao = 0;
	}

	//destroy vbo
	if (gVbo)
	{
		glDeleteBuffers(1, &gVbo);
		gVbo = 0;
	}


	//detach vertex shader from shader program object
	glDetachShader(gShaderProgramObject, gVertexShaderObject);

	glDetachShader(gShaderProgramObject, gGeometryShaderObject);

	//detach fragment shader from shader program object
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);

	//delete vertex shader object 
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;

	glDeleteShader(gGeometryShaderObject);
	gGeometryShaderObject = 0;


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

	if (gpFile)
	{
		fclose(gpFile);
		gpFile = NULL;
	}

}































































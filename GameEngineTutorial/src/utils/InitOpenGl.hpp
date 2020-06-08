#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef _WIN64
#include <Windows.h>
#define NOMIMAX
#define ERR_COLOR SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), 12);
#define END_ERR_COLOR  SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), 15);
#define OUT_ERR_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
#define END_OUT_ERR_COLOR  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#elif _WIN32
#error "project only supports 64 bit build not 32"
#else
#define ERR_COLOR std::cerr<<"\033[1;31mbold ";
#define END_ERR_COLOR  std::cerr <<" \033[0m\n";
#define OUT_ERR_COLOR  std::cout<<"\033[1;31mbold ";
#define END_OUT_ERR_COLOR  std::cout <<" \033[0m\n";
#endif


void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{  
	OUT_ERR_COLOR
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;

	END_OUT_ERR_COLOR
}


inline void InitializeGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		//filter output if you wish
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	glfwSetErrorCallback([](int error, const char* message) {
		ERR_COLOR std::cerr << "[ Error: " << error << " ] " << message << '\n'; END_ERR_COLOR
	});
}


inline GLFWwindow* getWindow(int width, int height, const char* title) throw()
{
	InitializeGLFW();
	GLFWwindow *window = glfwCreateWindow(800, 400, title, nullptr, nullptr);

	if (!window) { glfwTerminate(); throw std::runtime_error("window creation failed"); }
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) mutable
	{
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
	});
	//create context for opengl
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	//initiaize glew
	if (glewInit() != GLEW_OK) throw std::runtime_error("failed glewInit");
	std::cout << glGetString(GL_VERSION) << std::endl;


	return window;
}
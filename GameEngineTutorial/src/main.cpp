/*********************************************************************************
  Template project for openg gl includes glfw and glew libraries and some utils
*********************************************************************************/
#include "utils/InitOpenGl.hpp"
#define CAMERA_IMPLEMENTATION
#include "utils/simpleCamera.hpp"
#include "utils/utilities.hpp"
#include "utils/Timer.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "utils/Input.hpp"
#include "RenderEngine/Loader.h"
#include "RenderEngine/Renderer.h"
#include "ShaderProgram/StaticShader.h"
#include "Models/TexturedModel.h"
#include "ModelLoader/ModelObjParser.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <vector>

bool isInsideTriangle(std::vector<glm::vec3>& vertices,const glm::mat4& mat, const glm::vec3& mouse);
bool isInsideSquare(std::vector<glm::vec3>& vertices, const glm::mat4& mat, const glm::vec3& mouse);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
glm::vec3 handleMousePanning(GLFWwindow& window, int width, int height, double&, double&, bool&);
// mouse inside the square
bool isInside{ false };
bool button_clicked{ false };
glm::vec3 previous_mouse{ 0 };
glm::vec3 mouse_vec{0};
glm::vec3 mouse_vec2{ 0 };
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

int main()
{
	
	auto window = getWindow(SCR_WIDTH, SCR_HEIGHT, "openGl");
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) 
		{
			glfwGetFramebufferSize(window, &width, &height);
			SCR_WIDTH = width;
			SCR_HEIGHT = height;
			glViewport(0, 0, width, height);
		});
	Loader loader;
	Renderer renderer;
	std::vector<glm::vec3> vertices 
	{
		{-0.5f,  0.5f, 0.f},
		{-0.5f, -0.5f, 0.f},
	    {0.5f, -0.5f, 0.f},
	   {0.5f,  0.5f, 0.f}
	};
	std::vector<unsigned int> indices
	{
		0,1,2,
		0,1,3,
		3,1,2
	};

	std::vector< float> textureCoords
	{
		0,0,
		0,1,
		1,1,
		1,0
	};

	//auto modelData = readObjFile("3DModels/airbus/airbus.txt");
	ModelObjParser parser;
	/*Timer t;
	parser.ParseModelData(modelData);
	t.elapsed();*/
	RawModel model = loader.loadToVao(vertices,textureCoords,indices);
	ShaderProgram shaderProgram("Shaders/vertex.sh", "Shaders/fragment.sh");
	Texture texture(loader.load2DTexture("wall.jpg",GL_TEXTURE0), GL_TEXTURE0);
	TexturedModel texturedModel{ model, texture };
	// panning
	bool hasPanning{ false };
	double dx{0}, dy{0};

	while (!glfwWindowShouldClose(window))
	{
		renderer.clear();
		shaderProgram.useProgram();
		
		glm::mat4 pan_translation{ 1};
		auto panning = handleMousePanning(*window, SCR_WIDTH, SCR_HEIGHT,dx,dy,hasPanning );
		if (hasPanning)
			pan_translation = glm::translate(glm::mat4(1), panning);
		pan_translation = glm::scale(pan_translation, glm::vec3(0.6f, 0.2f, 1.f));
	    shaderProgram.setMatrix("transformationMatrix", pan_translation);
		double xpos{}, ypos{};
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec2 coordinates = screenToNormalizeDeviceCoordinates(xpos, ypos, SCR_WIDTH, SCR_HEIGHT);
		//std::cout << std::setprecision(5) << "x: " << coordinates.x << " y: " << coordinates.y << std::endl;
		
		//shaderProgram.setBool("is_inside", isInsideTriangle(vertices,pan_translation, glm::vec3(coordinates, 1.f)));
		isInside = isInsideSquare(vertices, pan_translation, glm::vec3(coordinates, 1.f));
	
		shaderProgram.setBool("is_inside", isInside);
		shaderProgram.setBool("button_clicked", button_clicked);
	
		renderer.render(texturedModel);
		shaderProgram.stopProgram();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

bool isInsideTriangle(std::vector<glm::vec3>& vertices,const glm::mat4& mat, const glm::vec3& mouse)
{
	std::vector<glm::vec3> transformedVertices{};
	transformedVertices.reserve(vertices.size());
	// transform the vertices
	for( int i =0; i < vertices.size();++i)
	   transformedVertices.push_back(mat * glm::vec4(vertices[i],1.f));

	glm::vec3 v0 = transformedVertices[1] - transformedVertices[0];
	glm::vec3 v1 = transformedVertices[2] - transformedVertices[0];
	glm::vec3 v2 = mouse - transformedVertices[0];

	// Compute dot products
	float dot00 = dot( v0,v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	float uv = u + v;

	return (u >= 0.000f && v >= 0.00f && uv < 1.000f);
}
glm::vec3 handleMousePanning(GLFWwindow& window, int width, int height, double& dx, double& dy,bool& haspanning)
{
	glm::vec3 panning{ 0 };
	int state = glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		double xpos{}, ypos{};
		glfwGetCursorPos(&window, &xpos, &ypos);
		
		mouse_vec.x = xpos;
		mouse_vec.y = ypos;
		
		if (previous_mouse.x != 0.0f || previous_mouse.y != 0.0f)
		{
			
			dx += mouse_vec.x - previous_mouse.x;
			dy += mouse_vec.y - previous_mouse.y;
			haspanning = (dx != 0.0f || dy != 0.0f);
		}
		
		previous_mouse.x = mouse_vec.x;
		previous_mouse.y = mouse_vec.y;
	}
	else if (state == GLFW_RELEASE)
	{
		previous_mouse.x = 0.f;
		previous_mouse.y = 0.f;

	}

	//transform to normalised device coordinates
	glm::vec<2,double> result = screenToNormalizeDeviceCoordinates(dx, dy, width, height);
	
	panning.x = result.x;
	panning.y = result.y;
	panning.z = 0.0f;
	return panning;
}


bool isInsideSquare(std::vector<glm::vec3>& vertices, const glm::mat4& mat, const glm::vec3& mouse)
{
	assert(vertices.size() == 4);
	std::vector<glm::vec3> transformedVertices;
	transformedVertices.reserve(4);

	int lowestVertex{0};
	int greatesVertex{ 0 };
	// transform the vertices
	for (int i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i].x >= vertices[greatesVertex].x && vertices[i].y >= vertices[greatesVertex].y)
			greatesVertex = i;
		else if( vertices[i].x <= vertices[lowestVertex].x && vertices[i].y <= vertices[lowestVertex].y)
		    lowestVertex = i;

		transformedVertices.push_back(mat * glm::vec4(vertices[i], 1.f));

	}

	return (mouse.x >= transformedVertices[lowestVertex].x) && (mouse.x <= transformedVertices[greatesVertex].x) &&
		mouse.y <= transformedVertices[greatesVertex].y &&( mouse.y >= transformedVertices[lowestVertex].y);	   
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	button_clicked = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && isInside);
}
/*********************************************************************************
  Template project for openg gl includes glfw and glew libraries and some utils
*********************************************************************************/
#include "utils/InitOpenGl.hpp"
#define CAMERA_IMPLEMENTATION
#include "utils/simpleCamera.hpp"
#include "utils/utilities.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "utils/Input.hpp"
#include "RenderEngine/Loader.h"
#include "RenderEngine/Renderer.h"
#include "ShaderProgram/StaticShader.h"
#include "Models/TexturedModel.h"
#include <stdexcept>
#include <iostream>
#include <vector>


int main()
{
	int SCR_WIDTH = 800;
	int SCR_HEIGHT = 400;
	auto window = getWindow(SCR_WIDTH, SCR_HEIGHT, "openGl");
	Loader loader;
	Renderer renderer;
	std::vector<float> vertices 
	{
		-0.5f,  0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		 0.5f,  0.5f, 0.f,
	};
	std::vector<unsigned int> indices
	{
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

	RawModel model = loader.loadToVao(vertices,textureCoords,indices);
	ShaderProgram shaderProgram("Shaders/vertex.sh", "Shaders/fragment.sh");
	Texture texture(loader.load2DTexture("wall.jpg",GL_TEXTURE0), GL_TEXTURE0);
	TexturedModel texturedModel{ model, texture };
	createTransformationMatrix(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));
	while (!glfwWindowShouldClose(window))
	{
		renderer.prepare();
		
		shaderProgram.useProgram();
		renderer.render(texturedModel);
		shaderProgram.stopProgram();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window, Camera& camera, float elapsedTime, float velocity)
{
	camera.CheckMouseMovement(*window);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		velocity += 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		if (velocity > 20.f)
			velocity -= 0.2f;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.MoveFront(elapsedTime, velocity);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.MoveBack(elapsedTime, velocity);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.MoveLeft(elapsedTime, velocity);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.MoveRight(elapsedTime, velocity);
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

#include "Renderer.h"
#include "../Models/TexturedModel.h"
#include <GL/glew.h>
Renderer::Renderer()
{
	glEnable(GL_DEPTH_TEST);
}
void Renderer::clear()
{
	glClearColor(0.4, 0.3, 0.3, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(TexturedModel & texturedModel)
{
	const RawModel& model = texturedModel.getRawModel();
	const Texture& modelTexture = texturedModel.getModelTexture();
	glBindVertexArray(model.getVaoID());
	glActiveTexture(modelTexture.getActiveSampler());
	glBindTexture(GL_TEXTURE_2D, modelTexture.getID());
	glDrawElements(GL_TRIANGLES,model.getVertexCount(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

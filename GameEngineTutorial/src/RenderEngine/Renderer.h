#pragma once
class TexturedModel;
class Renderer
{
public:
	Renderer();
	void clear();

	void render(TexturedModel & model);
};


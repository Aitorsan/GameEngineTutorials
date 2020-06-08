#pragma once
#include "ShaderProgram.h"

struct StaticShader : public ShaderProgram
{
	StaticShader():ShaderProgram("Shaders/vertex.sh", "Shaders/fragment.sh")
	{
	}
	virtual ~StaticShader(){}
};



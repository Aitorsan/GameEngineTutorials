#pragma once
#include <vector>
struct Mesh
{   //positions/texture/normals
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	int vertexSize, textureSize, normalsSize;
};

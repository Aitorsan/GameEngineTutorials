#pragma once
#include "../Models/RawModel.h"
#include <vector>
#include <glm/glm.hpp>


class Loader
{
	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> ebos;
	std::vector<unsigned int> textures;
public:
	~Loader();
	RawModel loadToVao(const std::vector<float>& positions, const std::vector<float>& textureCoords, const std::vector<unsigned int>& indices);
	RawModel loadToVao(const std::vector<glm::vec3>& positions, const std::vector<float>& textureCoords, const std::vector<unsigned int>& indices);

	unsigned int load2DTexture(const char* filename,unsigned int spot);

private:
	unsigned int createVAO();
	void storeDataInAttributeList(int attributeNumber,  int coordinatesSize,const std::vector<float>& data);
	void bindIndicesBuffer(const std::vector<unsigned int>& indices);
	void unbindVAO();

};


#include "Loader.h"
#include <GL/glew.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

RawModel Loader::loadToVao(const vector<float>& positions, const vector<float>&textureCoords, const vector<unsigned int>& indices)
{
	unsigned int vaoId = createVAO();
	bindIndicesBuffer(indices);
	storeDataInAttributeList(0,3, positions);
	storeDataInAttributeList(1,2, textureCoords);
	unbindVAO();

	return RawModel(vaoId,indices.size());
}

unsigned int Loader::load2DTexture(const char * filename, unsigned int spot)
{
	stbi_set_flip_vertically_on_load(true);
	std::string modelsfolder{ "3Dmodels/" };
	modelsfolder.append(filename);
	// create a 2D gexture
	int x, y, channels;
	unsigned char *texture_data = stbi_load(modelsfolder.data(), &x, &y, &channels, 0);
	if (!texture_data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	GLuint textID;
	glGenTextures(1, &textID);
	glActiveTexture(spot);
	glBindTexture(GL_TEXTURE_2D, textID);

	//texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Ansotropic filter
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, &texture_data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texture_data);

	textures.push_back(textID);
	return textID;
}

Loader::~Loader()
{
	glDeleteVertexArrays(vaos.size(), vaos.data());
	glDeleteBuffers(vbos.size(), vbos.data());
	glDeleteBuffers(ebos.size(), ebos.data());
	glDeleteTextures(textures.size(), textures.data());
}

unsigned int Loader::createVAO()
{//create buffer objects
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	vaos.push_back(VAO);

	return VAO;
}

void Loader::storeDataInAttributeList(int attributeNumber, int coordinatesSize, const std::vector<float>& data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	
	//vertexdata
	glEnableVertexAttribArray(attributeNumber);
	glVertexAttribPointer(attributeNumber, coordinatesSize, GL_FLOAT, GL_FALSE, coordinatesSize * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	vbos.push_back(VBO);
}

void Loader::bindIndicesBuffer(const std::vector<unsigned int>& indices)
{
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),indices.data(),GL_STATIC_DRAW);
}

void Loader::unbindVAO()
{
	glBindVertexArray(0);
}

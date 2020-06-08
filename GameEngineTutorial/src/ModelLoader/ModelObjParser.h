/**************************************************************************************************************************************
  This is an simple obj loader for 3D models. It just parses vertex positions, normals and 3D or 2D texture coordinates.
  
  There are two ways of getting data from the loader:

  1. Just called the GetIndices and GetModelData functions wich will return a vector of floats contaning all the vertex data
     notice you will have to get the vertex sizes as well.

  2. Define your on struct and call the fillVertexSizes,fillModelData, and fillIndexData to fill in the vertex data.
     The struct or class passed in must contain the following public members:
      
	  - std::vector<float> for the vertex data
	  - std::vector<unsigned int> for the index data;
	  - three int variables for the vertex/texture/normals size, if one of this vertex is missing it will be filled with a 0 value.
  
  ¡WARNING! : THE LOADER CANNOT LOAD:

  1. Compose models that do not have the same number of vertices
    
	 e.g: 1//2 1//3 2//3  and 1/2/3 2/3/5 3/4/8
	 
	 As we can see one of the models in the file has only normals and positions while the other has positions/texture/normals
	 this will crash the parser we can have one of the options only.( Maybe can be fixed in the future) 

  2. Negative indexing formtas : -1/-2/-3 are not supported ( no intention to support it) 

  3. Does not load materials ( Maybe can be added in the future) 

  3. Does not load vp vertices and l attributes ( no intention to add it) 

  Other issue of interest is that a complex model can take up to 2 minutes loading

**********************************************************************************************************************************/
#ifndef MODEL_OBJ_PARSER_H
#define MODEL_OBJ_PARSER_H

#include <string>
#include <vector>

namespace {
class ModelObjParserImpl;
}

class ModelObjParser
{
	ModelObjParserImpl* impl;
public:
	enum GENOPTION
	{
		GEN_INDICES = 2, NO_GEN_INDICES = 1
	};

	ModelObjParser();	
	~ModelObjParser();
	void ParseModelData(const std::string& modelData, GENOPTION option = GEN_INDICES);
	std::vector<unsigned int> GetIndices();
	std::vector<float>& GetModelDataForIndexing();
	std::vector<float>& GetModelDataWithNoIndices();
	void fillVertexSizes(int& vertexPositionSize, int& vertexTextureSize, int& vertexNormalSize);
	
	template <class Mesh> inline void fillModelData(Mesh& shape, std::vector<float> Mesh::* vertices, std::vector<unsigned int> Mesh::* indices)
	{
		shape.*vertices = GetModelDataForIndexing();
		shape.*indices = GetIndices();
	}
};

#endif //MODEL_OBJ_PARSER_H
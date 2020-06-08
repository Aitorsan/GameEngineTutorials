#include "ModelObjParser.h"
#include <algorithm>
#include <limits>
#include <sstream>
#include <map>

namespace
{
    const std::string VERTEX_POSITION{"v"};
	const std::string VERTEX_TEXTURE {"vt"};
	const std::string VERTEX_NORMAL  {"vn"};
	const std::string INDICES{"f"};
	const std::string EMTPY_STRING{" "};
	constexpr std::size_t TRIANGULAR_FACES{3};

	struct vec3
	{
		vec3(float all):vec3(all,all,all){}
		vec3(float xf,float yf, float zf) :x{ xf }, y{ yf }, z{ zf }{}
		float x, y, z;
	};

	struct vec2
	{
		vec2(float all) :vec2(all, all) {}
		vec2(float xf,float yf) :x{ xf }, y{ yf }{}
		float x, y;
	};

	struct Vertex
	{
		vec3 v;
		vec3 vt3D;
		vec3 vn;
		vec2 vt2D;
		unsigned int index;
		unsigned int vtIndex;
		unsigned int vnIndex;
		bool has3Dtexture = false;

		Vertex()
			: v{ std::numeric_limits<float>::max() }
			, vt2D{ std::numeric_limits<float>::max() }
			, vt3D{ std::numeric_limits<float>::max() }
			, vn{ std::numeric_limits<float>::max() }
			, index{ std::numeric_limits<unsigned int>::max() }
			, vtIndex{ std::numeric_limits<unsigned int>::max() }
			, vnIndex{ std::numeric_limits<unsigned int>::max() }
		{
		}

		inline bool operator==(const Vertex& other) const
		{
			return  (other.index == index && other.vtIndex == vtIndex && other.vnIndex == vtIndex && other.vnIndex == vnIndex) && (other.has3Dtexture == has3Dtexture);
		}
		inline bool operator<(const Vertex& other)  const
		{
			return index < other.index;
		}
	};

	/******************************************************************************
	Splits string into tokens with a given delimiter, and add them into a vector.
	NOTE: if a token is an empty string, meaning an extra space it will be romoved
	and will not be added to the list of tokens.
	Returns an empty container , if the delimiter separator was not found.
	********************************************************************************/
	template <class Container>
	inline Container& split_str(const std::string& str, Container& cont, char delim = ' ')
	{
		std::size_t current, previous = 0;
		current = str.find(delim);
		while (current != std::string::npos)
		{
			std::string s = str.substr(previous, current - previous);
			if (!s.empty())
			{
				s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
				s.erase(std::remove(s.end() - 1, s.end(), ' '), s.end());
				cont.push_back(std::move(s));
			}
			previous = current + 1;
			current = str.find(delim, previous);
		}
		if (!cont.empty())
			cont.push_back(str.substr(previous, current - previous));
		return cont;
	}

	template <typename T>
	T str_to_number(std::string& str)
	{
		T number;
		std::stringstream ss(str);
		ss >> number;
		if (ss.fail())
		{
			std::runtime_error e(str);
			throw e;
		}
		return number;
	}

	using Positions = std::vector<float>;
	using Normals = std::vector<float>;
	using Textures = std::vector<float>;
	using FaceIndices = std::vector<int>;

	struct ModelObjParserImpl
	{
		std::vector<unsigned int> Indices;
		std::vector<float> ModelRawData;
		std::vector<float> IndexedModelData;
		int NormVertexLength;
		int TextureVertexLength;
		void ParseObjData(const std::string& d, std::vector<Positions>&, std::vector<Textures>&, std::vector<Normals>&, std::vector<FaceIndices>&);
		void AddVertex(std::vector<std::vector<float>>& outVertices, std::vector<std::string>& vertexTokens);
		void AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens);
		void TransformQuadFacesToTriangularFaces(
			std::vector<FaceIndices>& fi,
			const std::vector<std::string>& posIndices,
			const std::vector<std::string>& textureIndices, 
			const std::vector<std::string>& normalIndices);
	};
}

ModelObjParser::ModelObjParser( )
	:impl{new ModelObjParserImpl }
{
}
ModelObjParser::~ModelObjParser()
{
	delete impl;
}

void ModelObjParser::ParseModelData(const std::string& modelData, GENOPTION option)
{
	std::vector<Positions> vPositions;
	std::vector<Normals> vNormals;
	std::vector<Textures> vTextures;
	std::vector<FaceIndices> fIndices;
	std::vector<Vertex> ModelRawDataVertices;
	
	impl->ParseObjData(modelData,vPositions,vTextures,vNormals,fIndices);

	if (vNormals.empty() && vTextures.empty())
	{
		// we have only positions
		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			Positions pos = vPositions[posIndex];
			v.index = posIndex;
			v.v = { pos[0], pos[1],pos[2] };
			ModelRawDataVertices.push_back(std::move(v));
		}
	}
	else if (vTextures.empty())// we have positions/Normals
	{
		impl->NormVertexLength = 3;
		
		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			const int& normIndex = indices.at(1);
			Positions pos = vPositions[posIndex];
			Normals norm = vNormals[normIndex];
			v.index = posIndex;
			v.vnIndex = normIndex;
			v.v = { pos[0], pos[1],pos[2] };
			v.vn = { norm[0], norm[1],norm[2] };

			ModelRawDataVertices.push_back(std::move(v));
		}
	}
	else if (vNormals.empty())// we have  positons/Textures
	{
		impl->TextureVertexLength = 2;

		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			const int& textIndex = indices.at(1);
			Positions pos = vPositions[posIndex];
			Textures text = vTextures[textIndex];
			v.index = posIndex;
			v.vtIndex = textIndex;
			v.v = { pos[0], pos[1],pos[2] };
			if (text.size() < 3)
				v.vt2D = { text[0], text[1] };
			else
			{
				v.vt3D = { text[0], text[1],text[2] };
				v.has3Dtexture = true;
			}
			ModelRawDataVertices.push_back(std::move(v));
		}

		Vertex& lastVertex = ModelRawDataVertices.back();

		if (lastVertex.has3Dtexture)
				impl->TextureVertexLength = 3;

	}
	else if( !vPositions.empty())// make sure nothing went wrong
	{
		//cool we have a full vertex the size is 3
		impl->NormVertexLength = 3;

		for (auto& indices : fIndices)
		{
			Vertex v;
			const int& posIndex = indices.front();
			const int& textIndex = indices.at(1);
			const int& normIndex = indices.at(2);
			Normals norm = vNormals[normIndex];
			Positions pos =vPositions[posIndex];
			Textures text = vTextures[textIndex];

			v.index = posIndex;
			v.vtIndex = textIndex;
			v.vnIndex = normIndex;
			v.v  = { pos[0], pos[1], pos[2] };
			v.vn = { norm[0], norm[1], norm[2] };

			if (text.size() < 3)
				v.vt2D = { text[0], text[1] };
			else
			{
				v.vt3D = { text[0], text[1],text[2] };
				v.has3Dtexture = true;
			}
			
			ModelRawDataVertices.push_back(std::move(v));
		}

		Vertex& lastVertex = ModelRawDataVertices.back();

		if (lastVertex.has3Dtexture)
			impl->TextureVertexLength = 3;
		else
			impl->TextureVertexLength = 2;
	}

	//create crresponding indexBuffers and remove duplicates 
	std::map<Vertex, int> indexMap;
    std::vector<Vertex> noRepeatedVerticesData;
	
	int currentIndex = 0;
	
	for (std::size_t i = 0; i < ModelRawDataVertices.size(); ++i)
	{
		Vertex& vt = ModelRawDataVertices[i];
		std::pair<std::map<Vertex, int>::iterator, bool> value = indexMap.insert(std::pair<Vertex, int>{vt, 0});
		// value.second == false if an equivalent key already existed
		if (value.second || i == 0)
		{
			indexMap[vt] = currentIndex;
			impl->Indices.push_back(currentIndex);
			noRepeatedVerticesData.push_back(vt);
			++currentIndex;
		}
		else if (vt == (value.first)->first)// the value already exist
		{
			impl->Indices.push_back((value.first)->second);
		}
		else// at this point the keys are seen as equal by the map because they have same index
		{
			// To avoid the map to remove the "similar" vertices , we change the index so we can have duplicate keys
			// because we really don't care about the values of the index in the map at this point we just use the map
			// as a look up so it is quicker than iterating through a vector 
			vt.index = -1;
			indexMap[(value.first)->first] = (value.first)->second;
			indexMap[vt] = currentIndex;
			impl->Indices.push_back(currentIndex);
			noRepeatedVerticesData.push_back(vt);
			++currentIndex;
		}
	}

	if (option & NO_GEN_INDICES)
	{
		// Vertex data with no indices to use with glDrawArrays()
		for (auto& vertex : ModelRawDataVertices)
		{
			impl->ModelRawData.push_back(vertex.v.x);
			impl->ModelRawData.push_back(vertex.v.y);
			impl->ModelRawData.push_back(vertex.v.z);

			if (impl->TextureVertexLength != 0)
			{
				if (vertex.has3Dtexture)
				{
					impl->ModelRawData.push_back(vertex.vt3D.x);
					impl->ModelRawData.push_back(vertex.vt3D.y);
					impl->ModelRawData.push_back(vertex.vt3D.z);
				}
				else
				{
					impl->ModelRawData.push_back(vertex.vt2D.x);
					impl->ModelRawData.push_back(vertex.vt2D.y);
				}
			}
			if (impl->NormVertexLength != 0)
			{
				impl->ModelRawData.push_back(vertex.vn.x);
				impl->ModelRawData.push_back(vertex.vn.y);
				impl->ModelRawData.push_back(vertex.vn.z);
			}
		}
	}
	
	if (option & GEN_INDICES)
	{
		// Vertex data with  indices to use with glDrawElements()
		for (auto& vertex : noRepeatedVerticesData)
		{
			impl->IndexedModelData.push_back(vertex.v.x);
			impl->IndexedModelData.push_back(vertex.v.y);
			impl->IndexedModelData.push_back(vertex.v.z);

			if (impl->TextureVertexLength != 0)
			{
				if (vertex.has3Dtexture)
				{
					impl->IndexedModelData.push_back(vertex.vt3D.x);
					impl->IndexedModelData.push_back(vertex.vt3D.y);
					impl->IndexedModelData.push_back(vertex.vt3D.z);
				}
				else
				{
					impl->IndexedModelData.push_back(vertex.vt2D.x);
					impl->IndexedModelData.push_back(vertex.vt2D.y);
				}
			}
			if (impl->NormVertexLength != 0)
			{
				impl->IndexedModelData.push_back(vertex.vn.x);
				impl->IndexedModelData.push_back(vertex.vn.y);
				impl->IndexedModelData.push_back(vertex.vn.z);
			}
		}
	}
}

void ModelObjParserImpl::ParseObjData(const std::string& data, std::vector<Positions>& vP, std::vector<Textures>& vT, 
	                                            std::vector<Normals>& vN, std::vector<FaceIndices>& fI)
{
	std::vector<std::string> tokens;
	split_str(data, tokens, '\n');
	// fill in the vectors
	for (std::string& token : tokens)
	{
		std::vector<std::string> splitedToken;
		split_str(token, splitedToken);
		//TODO: Could this be paralelized? 
		if (!splitedToken.empty())
		{
			auto& vertex_type = splitedToken.front();
			if (vertex_type == VERTEX_POSITION)
			{ 
				splitedToken.erase(splitedToken.begin());
				AddVertex(vP, splitedToken);
			}
			else if (vertex_type == VERTEX_TEXTURE)
			{
				splitedToken.erase(splitedToken.begin());
				AddVertex(vT, splitedToken);
			}
			else if (vertex_type == VERTEX_NORMAL)
			{
				splitedToken.erase(splitedToken.begin());
				AddVertex(vN, splitedToken);
			}
			else if (vertex_type == INDICES)
			{
				splitedToken.erase(splitedToken.begin());
				AddFaceIndices(fI, splitedToken);
			}
		}
	}
}

void ModelObjParserImpl::AddVertex(std::vector<std::vector<float>>& outVertices, std::vector<std::string>& vertexTokens)
{
	std::vector<float> verts;
	for (std::string& token : vertexTokens)
	{
		float v = str_to_number<float>(token);
		verts.push_back(v);
	}
	outVertices.push_back(std::move(verts));
}

void ModelObjParserImpl::AddFaceIndices(std::vector<FaceIndices>& fi, std::vector<std::string>& facesTokens)
{
		std::vector<std::string> posIndices;
		std::vector<std::string> textureIndices;
		std::vector<std::string> normalIndices;
		for ( std::size_t i = 0; i < facesTokens.size(); ++i)
		{
			std::string& token = facesTokens[i];

			std::vector<std::string> faceIndices;
			faceIndices = split_str(token, faceIndices, '/');
			std::size_t tSize = faceIndices.size();

			if (tSize == 0 && !token.empty())
			{  
				posIndices.push_back(token);
			}
			else if (tSize == 3)
			{
				posIndices.push_back(faceIndices.front());
				textureIndices.push_back(faceIndices.at(1));
				normalIndices.push_back(faceIndices.back());
			}
			else if (tSize == 2)
			{
				// count the number of / to understand if we have positions/ textures or positions/ nomals
				size_t n = std::count(token.begin(), token.end(), '/');
				if (n == 1) // positions/ textures
				{
					posIndices.push_back(faceIndices.front());
					textureIndices.push_back(faceIndices.back());
				}
				else if (n == 2)// positions/normals
				{
					posIndices.push_back(faceIndices.front());
					normalIndices.push_back(faceIndices.back());
				}
			}
		}

		//if the format is specified in quads we need to create triangular faces oursevles
		if (facesTokens.size() > TRIANGULAR_FACES)
		{
			//TODO: This is a pain would be worth to use GL_DRAW_STRIPS in this cases or paralelize this stuff
			TransformQuadFacesToTriangularFaces(fi, posIndices,textureIndices,normalIndices);
		}
		else
		{
			for (std::size_t i = 0; i < posIndices.size(); ++i)
			{
				FaceIndices faces{};
				faces.push_back(str_to_number<unsigned int>(posIndices[i]) - 1);
				if (!textureIndices.empty())
					faces.push_back(str_to_number<unsigned int>(textureIndices[i]) - 1);
				if (!normalIndices.empty())
					faces.push_back(str_to_number<unsigned int>(normalIndices[i]) - 1);
				fi.push_back(std::move(faces));
			}
		}
}

void ModelObjParserImpl::TransformQuadFacesToTriangularFaces(std::vector<FaceIndices>& fi, const std::vector<std::string>& posIndices,
																				const std::vector<std::string>& textureIndices,
																				const std::vector<std::string>& normalIndices)
{
	// transform in tirangular faces
	// eg: 1 2 3 4  -> 1 2 3 1 3 4
	std::vector<std::string> triangularIndexPositions{};
	std::size_t np = posIndices.size();
	for (std::size_t i = 0; i < np - 2; ++i)
	{
		triangularIndexPositions.push_back(posIndices[0]);
		triangularIndexPositions.push_back(posIndices[i + 1]);
		triangularIndexPositions.push_back(posIndices[i + 2]);
	}

	std::vector<std::string> triangularIndexTextures{};

	if (!textureIndices.empty())
	{
		std::size_t nt = textureIndices.size();
		for ( std::size_t i = 0; i < nt - 2; ++i)
		{
			triangularIndexTextures.push_back(textureIndices[0]);
			triangularIndexTextures.push_back(textureIndices[i + 1]);
			triangularIndexTextures.push_back(textureIndices[i + 2]);
		}
	}

	std::vector<std::string> triangularIndexNormals{};
	if (!normalIndices.empty())
	{
		std::size_t n = normalIndices.size();
		for (std::size_t i = 0; i < n - 2; ++i)
		{
			triangularIndexNormals.push_back(normalIndices[0]);
			triangularIndexNormals.push_back(normalIndices[i + 1]);
			triangularIndexNormals.push_back(normalIndices[i + 2]);
		}
	}
	
	for (std::size_t i = 0; i < triangularIndexPositions.size(); ++i)
	{
		FaceIndices faces{};
		faces.push_back(str_to_number<unsigned int>(triangularIndexPositions[i]) - 1);
		if (!triangularIndexTextures.empty() && i < triangularIndexTextures.size())
			faces.push_back(str_to_number<unsigned int>(triangularIndexTextures[i]) - 1);
		if (!triangularIndexNormals.empty() && i < triangularIndexNormals.size())
			faces.push_back(str_to_number<unsigned int>(triangularIndexNormals[i]) - 1);
		fi.push_back(std::move(faces));
	}
}

std::vector<unsigned int> ModelObjParser::GetIndices()
{
	return impl->Indices;
}

std::vector<float>& ModelObjParser::GetModelDataWithNoIndices()
{
	return impl->ModelRawData;
}

std::vector<float>& ModelObjParser::GetModelDataForIndexing()
{
	return impl->IndexedModelData;
}

void ModelObjParser::fillVertexSizes(int& vertexPositionSize, int& vertexTextureSize, int& vertexNormalSize)
{
	vertexPositionSize = 3;
	vertexTextureSize = impl->TextureVertexLength;
	vertexNormalSize = impl->NormVertexLength;
}
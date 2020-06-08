#pragma once
class RawModel
{
	int vaoID;
	int vertexCount;
public:

	RawModel(int vaoid, int vertexcount) :vaoID{ vaoid }, vertexCount{ vertexcount } {
	}
	int getVaoID() const { return vaoID; }
	int getVertexCount()const { return vertexCount; }
};


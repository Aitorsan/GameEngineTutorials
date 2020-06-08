#pragma once

#include "glm/glm.hpp"

class ShaderProgram
{
	int vertexShaderID;
	int fragmentShaderID;
	int programID;
public:
	ShaderProgram(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath);
	virtual ~ShaderProgram();
	int createShaderProgram();
	unsigned int loadShader(const char* ShaderSourcePath,unsigned int type);
	void useProgram();
	void stopProgram();
	// utility uniform functions
	int getUniformLocation(const char* name)const;
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVector3f(const char* name, const glm::vec3&  vector)const;
	void setMatrix(const char* name, const glm::mat4& matrix) const;
};


#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "glm/glm.hpp"
/******************************************************************************
   Splits string into tokens with a given delimiter, and add them into a vector.
   NOTE: if a token is an empty string, meaning an extra space it will be romoved
   and will not be added to the list of tokens.
   Returns an empty container , if the delimiter separator was not found.
********************************************************************************/
template <class Container>
inline Container& split_str(const std::string& str, Container& cont,char delim = ' ')
{
	std::size_t current, previous = 0;
	current = str.find(delim);
	while (current != std::string::npos)
	{
		std::string s = str.substr(previous, current - previous);
		if (!s.empty())
		{
			s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
			s.erase(std::remove(s.end()-1, s.end(), ' '), s.end());
			cont.push_back(std::move(s));
		}
		previous = current + 1;
		current = str.find(delim, previous);
	}
	if( !cont.empty())
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


inline std::string readObjFile(const std::string& file)
{
	std::fstream f(file);

	if (!f.is_open())
		return std::string();

	std::string data{ std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>() };

	return data;
}


inline glm::mat4 createTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
{
	glm::mat4 transforMationMatrix { 1 };

	/* this needs to be multplied from left to right 
	         
	          T * R * S 
	    If we start with the scale because the vector will be multiplied from the right side in the shader
		
		T*R*S * vector

		If we multiply it from the right side then the order would be as we expected because of non comutative 
		propertie of the matrices
		vector * S * R * T
	*/
	transforMationMatrix = glm::translate(transforMationMatrix, translation);
	if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0)
	{
		transforMationMatrix = glm::rotate(transforMationMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transforMationMatrix = glm::rotate(transforMationMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transforMationMatrix = glm::rotate(transforMationMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	}
	transforMationMatrix = glm::scale(transforMationMatrix, scale);
	
	return transforMationMatrix;
}
#pragma once
#include "Model.h"
#include "Texture.h"


namespace Utils
{
 
	GLuint CreateProgram(const ShaderProgram& shaders);
	Model LoadOBJModel( int programID, const std::string& filePath, const char* texturefilePath);
	glm::mat4 GenerateTranslationMatrix(glm::vec3 translation);
	glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees);
	glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis);
	std::string Load_File(const std::string& filePath);
	GLuint LoadFragmentShader(const std::string& filePath);
	GLuint LoadGeometryShader(const std::string& filePath);
	GLuint LoadVertexShader(const std::string& filePath);
	float GetElapsedTime();

};



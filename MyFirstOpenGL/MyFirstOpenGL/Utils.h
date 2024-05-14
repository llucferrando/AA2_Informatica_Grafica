#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"
#include "GameObject.h"
#include "Texture.h"


namespace Utils 
{
	Model* LoadOBJModel(GLuint _shaderProgram,  const std::string& filePath, const char* texturefilePath);
	glm::mat4 GenerateTranslationMatrix(glm::vec3 translation);
	glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees);
	glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis);
	std::string Load_File(const std::string& filePath);
	GLuint LoadFragmentShader(const std::string& filePath);
	GLuint LoadGeometryShader(const std::string& filePath);
	GLuint LoadVertexShader(const std::string& filePath);
	float GetElapsedTime();
}



#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Model.h"

struct ShaderProgram {
	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

class MyShaderProgram
{
public:
	MyShaderProgram();
	std::vector<GLuint> compiledPrograms;
	GLuint CreateProgram(const ShaderProgram& shaders);
	GLuint LoadFragmentShader(const std::string& filePath);
	GLuint LoadGeometryShader(const std::string& filePath);
	GLuint LoadVertexShader(const std::string& filePath);
	std::string Load_File(const std::string& filePath);
	glm::mat4 GenerateTranslationMatrix(glm::vec3 translation);
	glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees);
	glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis);
	Model LoadOBJModel(MyShaderProgram* program, int programID, const std::string& filePath, const char* texturefilePath);

	
};



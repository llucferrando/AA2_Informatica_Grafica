#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class GameObject
{

public:
	GameObject() = default;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 localVectorUp;
	

};




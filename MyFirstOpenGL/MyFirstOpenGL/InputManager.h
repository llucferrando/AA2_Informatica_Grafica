#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "Primitive.h"
#include "Camera.h"


class InputManager
{
	public:

	bool isStoped = false;
	bool primitiveFilled = false;
	bool pressedMChecked = false;
	bool pressedNChecked = false;
	int prevKeyMState = GLFW_RELEASE;
	int prevKeyNState = GLFW_RELEASE;
	int prevKey1State = GLFW_RELEASE;
	int prevKey2State = GLFW_RELEASE;
	int prevKey3State = GLFW_RELEASE;
	int prevKey4State = GLFW_RELEASE;
	int prevKeySpaceState = GLFW_RELEASE;

	void InputTransforms(GLFWwindow * window,  Camera * myCamera);

	
};


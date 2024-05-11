#include "InputManager.h"

void InputManager::InputTransforms(GLFWwindow * window, Primitive * cube, Primitive * ortoedro, Primitive * pyramid, Camera * myCamera)
{
	//Get keys to compare
	int currentKeyMState = glfwGetKey(window, GLFW_KEY_M);
	int currentKeyNState = glfwGetKey(window, GLFW_KEY_N);
	int currentKey1State = glfwGetKey(window, GLFW_KEY_1);
	int currentKey2State = glfwGetKey(window, GLFW_KEY_2);
	int currentKey3State = glfwGetKey(window, GLFW_KEY_3);
	int currentKey4State = glfwGetKey(window, GLFW_KEY_4);
	int currentKeySpaceState = glfwGetKey(window, GLFW_KEY_SPACE);

	//Check if the execution is stopped by user input
	if(!isStoped){
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			myCamera->position.y += 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			myCamera->position.y -= 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			myCamera->position.x -= 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			myCamera->position.x += 0.01f;
		}
		//M INPUTS
		if (currentKeyMState == GLFW_PRESS && prevKeyMState == GLFW_RELEASE){
			// Accelerate cube transforms velocity
			cube->setYVelocity(cube->getYVelocity() * 1.1f);
			cube->setFAngularVelocity(cube->getFAngularVelocity() * 1.1f);
			// Accelerate pyramide transforms velocity
			pyramid->setYVelocity(pyramid->getYVelocity() * 1.1f);
			pyramid->setFAngularVelocity(cube->getFAngularVelocity() * 1.1f);
			// Accelerate ortoedro transforms velocity
			ortoedro->setYVelocity(ortoedro->getYVelocity() * 1.1f);
			ortoedro->setFAngularVelocity(ortoedro->getFAngularVelocity() * 1.1f);
		}
		//N INPUTS
		if (currentKeyNState == GLFW_PRESS && prevKeyNState == GLFW_RELEASE){
			// Deaccelerate cube transforms velocity
			cube->setYVelocity(cube->getYVelocity() * 0.9f);
			cube->setFAngularVelocity(cube->getFAngularVelocity() * 0.9f);
			// Deaccelerate pyramid transforms velocity
			pyramid->setYVelocity(pyramid->getYVelocity() * 0.9f);
			pyramid->setFAngularVelocity(cube->getFAngularVelocity() * 0.9f);
			// Deaccelerate pyramid transforms velocity
			ortoedro->setYVelocity(ortoedro->getYVelocity() * 0.9f);
			ortoedro->setFAngularVelocity(ortoedro->getFAngularVelocity() * 0.9f);
		}

		//1 INPUTS
		if (currentKey1State == GLFW_PRESS && prevKey1State == GLFW_RELEASE && !primitiveFilled){
			primitiveFilled = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (currentKey1State == GLFW_PRESS && prevKey1State == GLFW_RELEASE && primitiveFilled) {
			primitiveFilled = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//2 INPUTS
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && prevKey2State == GLFW_RELEASE && cube->isRendering()){
			cube->setIsRendering(false);
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && prevKey2State == GLFW_RELEASE && !cube->isRendering()){
			cube->setIsRendering(true);
		}
		//3 INPUTS
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && prevKey3State == GLFW_RELEASE && pyramid->isRendering()){
			pyramid->setIsRendering(false);
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && prevKey3State == GLFW_RELEASE && !pyramid->isRendering()){
			pyramid->setIsRendering(true);
		}
		//4 INPUTS
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && prevKey4State == GLFW_RELEASE && ortoedro->isRendering()){
			ortoedro->setIsRendering(false);
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && prevKey4State == GLFW_RELEASE && !ortoedro->isRendering()){
			ortoedro->setIsRendering(true);
		}
	
	}
	
	//SPACE INPUTS
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && prevKeySpaceState == GLFW_RELEASE && !isStoped)
	{
		isStoped = true;

	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && prevKeySpaceState == GLFW_RELEASE && isStoped)
	{
		isStoped = false;
	}
	
	//UPDATE THE KEY STATES
	prevKeyMState = currentKeyMState;
	prevKeyNState = currentKeyNState;
	prevKey1State = currentKey1State;
	prevKey2State = currentKey2State;
	prevKey3State = currentKey3State;
	prevKey4State = currentKey4State;
	prevKeySpaceState = currentKeySpaceState;

}

#include "InputManager.h"

void InputManager::InputTransforms(GLFWwindow * window,  Camera * myCamera)
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
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			myCamera->position.z += 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			myCamera->position.z -= 0.01f;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			myCamera->fFov += 0.1f;
			if (myCamera->fFov > 180.f) {
				myCamera->fFov = 180.f;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			myCamera->fFov -= 0.1f;
			if (myCamera->fFov < 1.0f) {
				myCamera->fFov = 1.0f;
			}
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
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && prevKey2State == GLFW_RELEASE){
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && prevKey2State == GLFW_RELEASE ){
		}
		//3 INPUTS
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && prevKey3State == GLFW_RELEASE){
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && prevKey3State == GLFW_RELEASE ){
		}
		//4 INPUTS
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && prevKey4State == GLFW_RELEASE ){
			
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && prevKey4State == GLFW_RELEASE ){
			
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

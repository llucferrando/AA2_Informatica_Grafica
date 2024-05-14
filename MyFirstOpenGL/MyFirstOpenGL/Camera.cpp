#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.5f, 0.5f, 1.f);
	localVectorUp = glm::vec3(0.f, 1.f, 0.f);
	fFov = 45.f;
	fNear = 0.1f;
	fFar = 10.f;

}

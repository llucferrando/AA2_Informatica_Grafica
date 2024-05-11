#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	float fov;
	float aspectRatio;
	float fNear;
	float fFar;
	

};




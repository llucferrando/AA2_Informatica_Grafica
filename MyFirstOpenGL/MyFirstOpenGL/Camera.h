#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	float fFov;
	float aspectRatio;
	float fNear;
	float fFar;
	

};




#pragma once
#include <iostream>
#include "stb_image.h"
#include <GL/glew.h> 
#include <vector>

class Texture
{
public:

	Texture() = default;
	void InitializeTexture(const char* filePath);

};


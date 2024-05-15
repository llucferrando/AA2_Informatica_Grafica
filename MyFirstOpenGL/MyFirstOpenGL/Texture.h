#pragma once
#include <iostream>
#include "stb_image.h"
#include <GL/glew.h> 
#include <vector>

class Texture
{
public:
	
	Texture(const char* filePath, const std::vector<float>& uvs);
	void InitializeTexture();
private:
	
	const char* _filePath;
	
	const std::vector<float>& _uvs;

	
};


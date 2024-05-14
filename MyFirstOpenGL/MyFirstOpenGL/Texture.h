#pragma once
#include <iostream>
#include "stb_image.h"
#include <GL/glew.h> 
#include <vector>

class Texture
{
public:
	Texture() = default;
	Texture(const char* filePath, const std::vector<float>& uvs);
	void InitializeTexture();
private:
	int width, height, nrChannels;
	const char* _filePath;
	GLuint _uvVBO, _textureID;
	const std::vector<float>& _uvs;

	
};


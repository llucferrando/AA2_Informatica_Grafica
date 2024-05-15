#ifndef MODEL_H
#define MODEL_H
#include "GameObject.h"
#include "Texture.h"
#include <vector>
#include "stb_image.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>
#include "Camera.h"


class Model : public GameObject
{
public:
	Model(const char* textureFilePath, GLuint shaderProg, const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
	virtual void Render();
	virtual void UseProgram();
	Camera* myCamera;


protected:
	GLuint VAO, VBO, uvVBO;
	unsigned int numVertexs;
	Texture * _texture;
	
	GLuint _programID;

};


#endif 
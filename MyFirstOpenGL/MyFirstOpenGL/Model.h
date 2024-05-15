#ifndef MODEL_H
#define MODEL_H
#include "GameObject.h"
#include "Texture.h"
#include "Camera.h"
#include "MyShaderProgram.h"


class Model : public GameObject
{
public:
	Model(int programID, const char* textureFilePath, const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
	virtual void Render();
	virtual void UseProgram();
	Camera* myCamera;
	


protected:
	GLuint VAO, VBO, uvVBO;
	MyShaderProgram* myProgram;
	
	unsigned int numVertexs;
	Texture * _texture;
	int _programID;
	

};


#endif 
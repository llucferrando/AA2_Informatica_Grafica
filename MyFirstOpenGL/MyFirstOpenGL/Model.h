#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>
#include "Texture.h"
#include "ProgramManager.h"
#include "Camera.h"
#include "GameObject.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 880

class Model : public GameObject {
public:
    Model( int IDProgram, const char* filePath,const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render();
    void UseProgram();
    Texture* _texture;
    Camera* myCamera;

private:
    GLuint VAO, VBO, uvVBO, normalsVBO;
    unsigned int numVertexs;
    int _programID;
};

#endif

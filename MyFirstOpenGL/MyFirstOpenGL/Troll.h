
#ifndef TROLL_H
#define TROLL_H
#include "GameObject.h"
#include <vector>
#include <GL/glew.h>
class Troll : public GameObject
{
public:
	Troll(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
	void Render() const;

private:
	GLuint VAO, VBO;
	unsigned int numVertexs;
};
#endif 




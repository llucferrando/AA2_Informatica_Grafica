#include "Model.h"
#include <iostream>

Model::Model(int IDProgram,const char* filePath,const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals) {
    
    _programID = IDProgram;
    //Almaceno la cantidad de vertices que habra
    this->numVertexs = vertexs.size() / 3;

    //Generamos VAO/VBO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->uvVBO);
    glGenBuffers(1, &this->normalsVBO);

    //Defino el VAO creado como activo
    glBindVertexArray(this->VAO);

    //Defino el VBO de las posiciones como activo, le paso los datos y lo configuro
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(float), vertexs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Definimos el VBO de las coordenadas de textura como activo, le pasamos los datos y lo configuramos
    glBindBuffer(GL_ARRAY_BUFFER, this->uvVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //Defino el VBO de las posiciones como activo, le paso los datos y lo configuro
    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Activamos ambos atributos para ser usados
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //Desvinculamos VAO y VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);  
    
    _texture->InitializeTexture(filePath);

   
}

void Model::Render() {

    //UseProgram();
    //Vinculo su VAO para ser usado
    glBindVertexArray(this->VAO);

    // Dibujamos
    glDrawArrays(GL_TRIANGLES, 0, this->numVertexs);

    //Desvinculamos VAO
    glBindVertexArray(0);

}

void Model::UseProgram()
{
    GLuint myProgram = ProgramManager::getInstance().compiledPrograms[_programID];
    glUseProgram(myProgram);

    glm::mat4 translationMatrix = ProgramManager::getInstance().GenerateTranslationMatrix(position + glm::vec3(0.f, 0.f, 0.f));
    glm::mat4 rotationMatrix = ProgramManager::getInstance().GenerateRotationMatrix(glm::vec3(1.f, 1.f, 1.f), rotation.x);
    glm::mat4 scaleMatrix = ProgramManager::getInstance().GenerateScaleMatrix(scale);
    glm::mat4 view = glm::lookAt(myCamera->position, myCamera->position + glm::vec3(0.f, 0.f, -1.f), myCamera->localVectorUp);
    glm::mat4 projection = glm::perspective(myCamera->fFov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    glUniform2f(glGetUniformLocation(myProgram, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniform1i(glGetUniformLocation(myProgram, "textureSampler"), 0);
    glUniformMatrix4fv(glGetUniformLocation(myProgram, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(myProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(myProgram, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    glUniformMatrix4fv(glGetUniformLocation(myProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(myProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUseProgram(0);
    glDeleteProgram(myProgram);
}

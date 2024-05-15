#include "Texture.h"

Texture::Texture(const char* filePath, const std::vector<float>& uvs) : 
    _filePath(filePath), _uvs(uvs)
{
}

void Texture::InitializeTexture()
{
    int width, height, nrChannels;
	unsigned char* textureInfo = stbi_load(_filePath, &width, &height, &nrChannels, 0);
    //Definimos canal de textura activo
    glActiveTexture(GL_TEXTURE0);

    GLuint _textureID;

    glGenTextures(1, &_textureID);

    //Vinculamos texture
    glBindTexture(GL_TEXTURE_2D, _textureID);

    //Configurar textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Cargar imagen a la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureInfo);

    //Generar mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    //Liberar memoria de la imagen cargada
    stbi_image_free(textureInfo);
}



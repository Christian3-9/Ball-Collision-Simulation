#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <QOpenGLExtraFunctions>

class Texture:protected QOpenGLExtraFunctions
{
public:
    Texture();
    void create(unsigned char* data, int width, int height);
    void use();
    void destroy();
protected:
    unsigned int textureId;
};

#endif // TEXTURE_HPP

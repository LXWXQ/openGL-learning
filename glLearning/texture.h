#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
#include <string>
#include <glad/glad.h>
class Texture
{
public:
	Texture(const std::string& texturePath);
	~Texture();
	std::uint32_t getTexture() const { return texture; }
private:
	std::uint32_t texture;
};
#endif // !__TEXTURE_H__


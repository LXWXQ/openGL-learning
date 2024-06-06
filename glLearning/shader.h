#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
class Shader
{
public:
	Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
	~Shader();
	void checkShaderStatus(const std::uint32_t& shaderID,const std::string& shaderType);
	void checkProgramStatus();
	std::uint32_t getProgram() { return programIndex; }
	void Use();
	void addTexture(const std::uint32_t& texture,const std::string& textureUniform);
	void updateTimeColor();
private:
	void activeTextures();
	uint32_t programIndex;
	std::string vertexShaderSource;
	std::string fragementShaderSource;
	std::vector<std::uint32_t> textures;
};
#endif // !__SHADER_H__


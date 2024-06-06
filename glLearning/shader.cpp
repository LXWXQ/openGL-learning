#include"shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    textures.clear();
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkShaderStatus(vertex,"vertexShader");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkShaderStatus(fragment,"fragmentShader");
    // shader Program
    programIndex = glCreateProgram();
    glAttachShader(programIndex, vertex);
    glAttachShader(programIndex, fragment);
    glLinkProgram(programIndex);
    checkProgramStatus();
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{

}

void Shader::Use()
{
    //activeTextures();
    glUseProgram(programIndex);
}

void Shader::addTexture(const std::uint32_t& texture, const std::string& textureUniform)
{
    if (textures.size() > 32)//×î¶à32ÕÅÌùÍ¼
        return;
    glUniform1i(glGetUniformLocation(programIndex, textureUniform.c_str()), textures.size());
    textures.emplace_back(texture);
}

void Shader::activeTextures()
{
    if (textures.empty())
        return;
    for (std::uint32_t i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0+(int)i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}

void Shader::updateTimeColor()
{
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(programIndex, "timeColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

void Shader::checkShaderStatus(const std::uint32_t& shaderID,const std::string& shaderType)
{
    int success;
    char infoLog[1024];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

void Shader::checkProgramStatus()
{
    int success;
    char infoLog[1024];
    glGetProgramiv(programIndex, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programIndex, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
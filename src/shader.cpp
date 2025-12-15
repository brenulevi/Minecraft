#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    _id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);

    int success;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(_id, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Shader program linking failed: ") + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(_id);
}

void Shader::use() const
{
    glUseProgram(_id);
}

std::string Shader::loadShaderSource(const char *path) const
{
    std::ifstream shaderFile;
    shaderFile.open(path);

    if(!shaderFile.is_open())
        throw std::runtime_error(std::string("Failed to open shader file: ") + path);

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    return shaderStream.str();
}

unsigned int Shader::compileShader(unsigned int type, const char *source) const
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Shader compilation failed: ") + infoLog);
    }

    return shader;
}

int Shader::getUniformLocation(const std::string &name)
{
    if(_uniformLocationCache.find(name) == _uniformLocationCache.end())
    {
        int location = glGetUniformLocation(_id, name.c_str());
        _uniformLocationCache[name] = location;
    }

    return _uniformLocationCache[name];
}

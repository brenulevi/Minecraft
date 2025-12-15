#pragma once

#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    inline void setUniform1i(const std::string& name, int value)
    {
        glUniform1i(getUniformLocation(name), value);
    }

    inline void setUniform1f(const std::string& name, float value)
    {
        glUniform1f(getUniformLocation(name), value);
    }

    inline void setUniform3f(const std::string& name, float v0, float v1, float v2)
    {
        glUniform3f(getUniformLocation(name), v0, v1, v2);
    }

    inline void setUniformMat4f(const std::string& name, const float* matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix);
    }

private:
    std::string loadShaderSource(const char* path) const;
    unsigned int compileShader(unsigned int type, const char* source) const;
    int getUniformLocation(const std::string& name);

private:
    unsigned int _id;
    std::unordered_map<std::string, int> _uniformLocationCache;
};
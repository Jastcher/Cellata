#include "computeShader.h"
#include <fstream>
#include <iostream>
#include <sstream>

ComputeShader::ComputeShader()
{
}
ComputeShader::ComputeShader(const char *filePath)
{
  std::string source = LoadFile(filePath);
  Init(source, filePath);
}
ComputeShader::~ComputeShader()
{
}

std::string ComputeShader::LoadFile(const char *filePath)
{

  std::ifstream stream;
  stream.open(filePath);
  if (!stream.is_open())
  {
    std::cout << "Failed to open " << filePath << std::endl;
  }

  std::stringstream buffer;
  buffer << stream.rdbuf();

  return buffer.str();
}

void ComputeShader::Init(std::string source, const char *filePath)
{
  const char *cSource = source.c_str();
  GLuint shader       = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(shader, 1, &(cSource), NULL);
  glCompileShader(shader);
  CompileErrors(shader, "GEOMETRY", filePath);

  id = glCreateProgram();
  glAttachShader(id, shader);
  glLinkProgram(id);
  CompileErrors(shader, "GEOMETRY PROGRAM", filePath);

  // Cleanup
  glDeleteShader(shader);
}

void ComputeShader::Activate() const
{
  glUseProgram(id);
}

void ComputeShader::Dispatch(unsigned int x, unsigned int y) const
{
  glDispatchCompute(x, y, 1);
}

void ComputeShader::SetInt(const char *uniform, int unit) const
{
  glUniform1i(glGetUniformLocation(id, uniform), unit);
}

void ComputeShader::SetFloat(const char *uniform, float unit) const
{
  glUniform1f(glGetUniformLocation(id, uniform), unit);
}
void ComputeShader::SetVec2(const char *uniform, glm::vec2 unit) const
{
  glUniform2f(glGetUniformLocation(id, uniform), unit.x, unit.y);
}

const std::string ComputeShader::CompileErrors(unsigned int shader, const char *type, const char *shaderName)
{
  GLint hasCompiled;
  char infoLog[1024] = {""};
  if (type != "PROGRAM")
  {
    // Returns a parameter from a shader object - shader, pname, *params
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE)
    {
      // Returns the information log for a shader object - shader,
      // maxLength, length, infoLog
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << shaderName << ": SHADER COMPILATION ERROR:  " << type << std::endl;
    }
  }
  else
  {
    // Returns a parameter from a shader object - shader, pname, *params
    glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE)
    {
      // Returns the information log for a shader object - shader,
      // maxLength, length, infoLog
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << shaderName << ": SHADER LINKING ERROR FOR: " << type << std::endl;
    }
  }

  std::cout << infoLog << std::endl;

  return std::string(infoLog);
}

#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <string>
class ComputeShader
{
public:
  ComputeShader();
  ComputeShader(const char *filePath);
  ~ComputeShader();

  void Init(std::string source, const char *filePath);
  void Dispatch(unsigned int x, unsigned int y);
  void Activate();

  std::string LoadFile(const char *filePath);
  const std::string CompileErrors(unsigned int shader, const char *type,
                                  const char *shaderName);

  void SetInt(const char *uniform, int unit);
  void SetFloat(const char *uniform, float unit);
  void SetVec2(const char *uniform, glm::vec2 unit);

  GLuint id;
};

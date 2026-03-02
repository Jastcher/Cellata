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
  void Dispatch(int width, int height, int localX = 8, int localY = 8) const;
  void Activate() const;

  std::string LoadFile(const char *filePath);
  const std::string CompileErrors(unsigned int shader, const char *type, const char *shaderName);

  void SetInt(const char *uniform, int unit) const;
  void SetFloat(const char *uniform, float unit) const;
  void SetVec2(const char *uniform, glm::vec2 unit) const;

  GLuint id;
};

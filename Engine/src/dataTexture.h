#pragma once
#include "window.h"

#include <GL/gl.h>
class DataTexture
{
public:
  DataTexture();
  DataTexture(int width, int height);
  ~DataTexture();

  GLuint id = 0;

  GLuint format   = GL_RGBA8UI;
  GLuint dataType = GL_UNSIGNED_BYTE;

  void Init(int width, int height);
  void Resize(int width, int height);

  void Bind(GLuint unit);
};

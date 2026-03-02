#include "dataTexture.h"
#include "window.h"
#include <iostream>

DataTexture::DataTexture()
{
}
DataTexture::DataTexture(int width, int height)
{
  Init(width, height);
}

DataTexture::~DataTexture()
{

  glDeleteTextures(1, &id);
}

void DataTexture::Init(int width, int height)
{

  glCreateTextures(GL_TEXTURE_2D, 1, &id);
  // Allocate storage (RGBA32F is 4 floats per pixel)
  glTextureStorage2D(id, 1, format, width, height);

  // Set wrapping to Clamp to Border
  glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // Define the border color
  float borderColor[] = {0.1f, 0.1f, 0.1f, 1.0f};
  glTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, borderColor);

  // Set parameters as usual
  glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GLuint clearColor[] = {0, 0, 0, 0};
  glClearTexImage(id, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, clearColor);
}

void DataTexture::Bind(GLuint unit)
{
  glBindImageTexture(unit, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);
}

void DataTexture::Resize(int width, int height)
{

  if (id != 0)
  {
    // 1. Unbind from common slots just in case
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_ONLY, format);

    // 2. Delete
    glDeleteTextures(1, &id);
    id = 0; // Reset to 0 so we don't use a junk ID
  }
  Init(width, height);
}

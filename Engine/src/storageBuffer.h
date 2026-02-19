#pragma once

#include "window.h"
class StorageBuffer
{
public:
  StorageBuffer();
  ~StorageBuffer();

  void Init(int size, void *data);

  void Bind(GLuint slot);
  void Update(int sizeBytes, void *data);

  GLuint id;
};

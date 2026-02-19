

#include "storageBuffer.h"
#include "window.h"

StorageBuffer::StorageBuffer()
{
}
StorageBuffer::~StorageBuffer()
{
}

void StorageBuffer::Init(int sizeBytes, void *data)
{
  glGenBuffers(1, &id);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeBytes, data, GL_DYNAMIC_DRAW);
}

void StorageBuffer::Update(int sizeBytes, void *data)
{
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeBytes, data);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void StorageBuffer::Bind(GLuint slot)
{
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, id);
}

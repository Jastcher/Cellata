#include "automaton.h"
#include "dataTexture.h"
#include "imgui.h"
#include "window.h"
#include <iostream>

Automaton::Automaton()
{
  m_RenderDataTexture.format = GL_RGBA8;
}

Automaton::~Automaton()
{
}

void Automaton::OnDraw(float mouseX, float mouseY)
{
  DrawGPU(mouseX, mouseY);
}

void Automaton::DrawGPU(float mouseX, float mouseY)
{
  computeDraw.Activate();
  computeDraw.SetVec2("u_MousePos", glm::vec2(mouseX, mouseY));

  computeDraw.SetFloat("radius", drawRadius);

  computeDraw.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Automaton::DrawCPU(float mouseX, float mouseY)
{
  glBindTexture(GL_TEXTURE_2D, m_DataTexture.id);

  int x                = mouseX * width;
  int y                = mouseY * height;
  unsigned char data[] = {0, 0, 0, 0};

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                // Mipmap level
                  x, y,             // x and y offset
                  1, 1,             // Dimensions of the area to update
                  GL_RGBA8UI,       // Format of the pixel data on CPU
                  GL_UNSIGNED_BYTE, // Data type of the pixel data
                  data              // Pointer to the actual data on the CPU
  );
}

void Automaton::Step()
{
  for (const auto &sim : computeSims)
  {

    sim.Activate();

    m_DataTexture.Bind(0);
    m_NextDataTexture.Bind(1);

    sim.Dispatch(width / 8, height / 8);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    GLuint temp          = m_DataTexture.id;
    m_DataTexture.id     = m_NextDataTexture.id;
    m_NextDataTexture.id = temp;
  }
}

void Automaton::Render()
{
  computeRender.Activate();

  m_DataTexture.Bind(0);
  m_RenderDataTexture.Bind(1);

  computeRender.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
void Automaton::Resize(int _width, int _height)
{
  width  = _width;
  height = _height;
  // has to resize before doing anything with data textures
  m_DataTexture.Resize(width, height);
  m_NextDataTexture.Resize(width, height);
  m_RenderDataTexture.Resize(width, height);
}

void Automaton::UI()
{
  ImGui::Begin("EXAMPLE");

  ImGui::Text("Automaton");

  ImGui::End();
}

GLuint Automaton::GetRenderDataTextureID()
{
  return m_RenderDataTexture.id;
}

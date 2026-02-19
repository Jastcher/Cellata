#include "automaton.h"
#include "dataTexture.h"
#include "imgui.h"
#include <iostream>

Automaton::Automaton()
{
  m_RenderDataTexture.format = GL_RGBA8;
}

Automaton::~Automaton()
{
}

void Automaton::Draw(int mouseX, int mouseY)
{
  computeDraw.Activate();
  computeDraw.SetVec2("u_MousePos", glm::vec2(mouseX, mouseY));

  computeDraw.SetFloat("radius", drawRadius);

  computeDraw.Dispatch(m_Width / 8, m_Height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Automaton::Step()
{
  computeSim.Activate();

  m_DataTexture.Bind(0);
  m_NextDataTexture.Bind(1);

  computeSim.Dispatch(m_Width / 8, m_Height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  GLuint temp          = m_DataTexture.id;
  m_DataTexture.id     = m_NextDataTexture.id;
  m_NextDataTexture.id = temp;
}

void Automaton::Render()
{
  computeRender.Activate();

  m_DataTexture.Bind(0);
  m_RenderDataTexture.Bind(1);

  computeRender.Dispatch(m_Width / 8, m_Height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
void Automaton::Resize(int width, int height)
{
  // has to resize before doing anything with data textures
  m_DataTexture.Resize(width, height);
  m_NextDataTexture.Resize(width, height);
  m_RenderDataTexture.Resize(width, height);
  m_Width  = width;
  m_Height = height;
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

#include "langtonsAnt.h"
#include "imgui.h"

LangtonsAnt::LangtonsAnt() : Automaton()
{

  computeSims.push_back(ComputeShader("../Sandbox/Langtons ant/sim.comp"));
  computeRender = ComputeShader("../Sandbox/Langtons ant/render.comp");

  label = "LangtonsAnt";

  Resize(64, 64);

  DrawCPU(0.5f, 0.5f);
}

void LangtonsAnt::UI()
{
  ImGui::Begin("hemlo");

  ImGui::Text("zemla s makom");

  ImGui::End();
}

void LangtonsAnt::OnDraw(float mouseX, float mouseY)
{
  DrawCPU(mouseX, mouseY);
}

// Draws ant
void LangtonsAnt::DrawCPU(float mouseX, float mouseY)
{
  glBindTexture(GL_TEXTURE_2D, m_DataTexture.id);

  int x = mouseX * width;
  int y = mouseY * height;

  unsigned char data[] = {0, 1, 1, 0};

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                // Mipmap level
                  x, y,             // x and y offset
                  1, 1,             // Dimensions of the area to update
                  GL_RGBA_INTEGER,  // Format of the pixel data on CPU
                  GL_UNSIGNED_BYTE, // Data type of the pixel data
                  data              // Pointer to the actual data on the CPU
  );
}

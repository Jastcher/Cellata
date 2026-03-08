#include "wireworld.h"
#include "imgui.h"

Wireworld::Wireworld() : Automaton()
{

  computeSims.push_back(ComputeShader("../Sandbox/Wireworld/sim.comp"));
  computeRender = ComputeShader("../Sandbox/Wireworld/render.comp");

  label = "Wireworld";

  Resize(16, 16);
}

void Wireworld::UI()
{
  ImGui::Begin("hemlo");

  ImGui::SliderFloat("radius", &drawRadius, 0.1f, 50.0f);

  ImGui::Combo(selectionNames[selection], &selection, selectionNames.data(), selectionNames.size());

  ImGui::End();
}

void Wireworld::OnDraw(float mouseX, float mouseY)
{
  DrawCPU(mouseX, mouseY);
}

void Wireworld::DrawCPU(float mouseX, float mouseY)
{
  glBindTexture(GL_TEXTURE_2D, m_DataTexture.id);

  int x = mouseX * width;
  int y = mouseY * height;

  unsigned char data[] = {static_cast<unsigned char>(selection), 0, 0, 0};

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                // Mipmap level
                  x, y,             // x and y offset
                  1, 1,             // Dimensions of the area to update
                  GL_RGBA_INTEGER,  // Format of the pixel data on CPU
                  GL_UNSIGNED_BYTE, // Data type of the pixel data
                  data              // Pointer to the actual data on the CPU
  );
}

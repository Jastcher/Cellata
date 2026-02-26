#include "conway.h"
#include "imgui.h"

Conway::Conway() : Automaton()
{

  computeSims.push_back(ComputeShader("../Sandbox/Conway/sim.comp"));
  computeRender = ComputeShader("../Sandbox/Conway/render.comp");

  label = "Conway";

  Resize(16, 16);
}

void Conway::UI()
{
  ImGui::Begin("hemlo");

  ImGui::SliderFloat("radius", &drawRadius, 0.1f, 50.0f);

  ImGui::Checkbox("Alive", &selection);

  ImGui::End();
}

void Conway::OnDraw(float mouseX, float mouseY)
{
  DrawCPU(mouseX, mouseY);
}

void Conway::DrawCPU(float mouseX, float mouseY)
{
  glBindTexture(GL_TEXTURE_2D, m_DataTexture.id);

  int x = mouseX * width;
  int y = mouseY * height;

  unsigned char data[] = {selection, 0, 0, 0};

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                // Mipmap level
                  x, y,             // x and y offset
                  1, 1,             // Dimensions of the area to update
                  GL_RGBA_INTEGER,  // Format of the pixel data on CPU
                  GL_UNSIGNED_BYTE, // Data type of the pixel data
                  data              // Pointer to the actual data on the CPU
  );
}

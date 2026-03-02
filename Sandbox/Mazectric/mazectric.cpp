#include "mazectric.h"
#include "imgui.h"

Mazectric::Mazectric() : Automaton()
{

  computeSims.push_back(ComputeShader("../Sandbox/Mazectric/sim.comp"));
  computeRender = ComputeShader("../Sandbox/Mazectric/render.comp");

  label = "Mazectric";

  // Resize(16, 16);
  dynamicResize = true;
}

void Mazectric::UI()
{
  ImGui::Begin("hemlo");

  ImGui::Checkbox("Alive", &selection);
  static int distrib = 10;
  ImGui::SliderInt("Distrib", &distrib, 2, 1000);
  if (ImGui::Button("Randomize")) Randomize(distrib);

  ImGui::End();
}

void Mazectric::OnDraw(float mouseX, float mouseY)
{
  DrawCPU(mouseX, mouseY);
}

void Mazectric::DrawCPU(float mouseX, float mouseY)
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
void Mazectric::Randomize(int distrib)
{
  // For a 512x512 RGBA8 texture
  std::vector<unsigned char> randomData(width * height * 4);

  for (int i = 0; i < randomData.size(); ++i)
  {
    if (rand() % distrib == 0)
      randomData[i] = 1;
    else
      randomData[i] = 0;
  }

  glBindTexture(GL_TEXTURE_2D, m_DataTexture.id);

  glTexSubImage2D(GL_TEXTURE_2D,    // Target
                  0,                // Mipmap level
                  0, 0,             // x-offset, y-offset (start at bottom-left)
                  width, height,    // Width and height of the region to update
                  GL_RGBA_INTEGER,  // Format of the data you are sending
                  GL_UNSIGNED_BYTE, // Type of the data
                  randomData.data() // Pointer to the CPU buffer
  );
}

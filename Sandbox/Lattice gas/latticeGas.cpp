#include "latticeGas.h"
#include "computeShader.h"
#include "imgui.h"
#include "imgui_internal.h"

LatticeGas::LatticeGas() : Automaton()
{

  computeSims.push_back(ComputeShader("../Sandbox/Lattice gas/propagation.comp"));
  computeSims.push_back(ComputeShader("../Sandbox/Lattice gas/collision.comp"));
  computeRender = ComputeShader("../Sandbox/Lattice gas/render.comp");
  computeDraw   = ComputeShader("../Sandbox/Lattice gas/draw.comp");

  label         = "Lattice Gas";
  dynamicResize = true;
  // Resize(32, 32);
}

void LatticeGas::UI()
{
  ImGui::Begin("hemlo");

  ImGui::SliderFloat("radius", &drawRadius, 0.1f, 50.0f);

  if (ImGui::Selectable("empty", direction == 0)) direction = 0;
  if (ImGui::Selectable("up", direction == 1)) direction = 1;
  if (ImGui::Selectable("right", direction == 2)) direction = 2;
  if (ImGui::Selectable("down", direction == 3)) direction = 3;
  if (ImGui::Selectable("left", direction == 4)) direction = 4;

  if (ImGui::Checkbox("GPUDraw", &gpuDraw))
    ;

  if (ImGui::Button("Randomize")) Randomize();

  ImGui::End();
}

void LatticeGas::OnDraw(float mouseX, float mouseY)
{
  if (gpuDraw)
    DrawGPU(mouseX, mouseY);
  else
    DrawCPU(mouseX, mouseY);
}
void LatticeGas::DrawGPU(float mouseX, float mouseY)
{
  computeDraw.Activate();
  m_DataTexture.Bind(0);

  computeDraw.SetVec2("u_MousePos", glm::vec2(mouseX, mouseY));
  computeDraw.SetFloat("radius", drawRadius);

  computeDraw.SetInt("id", direction);
  computeDraw.SetInt("width", width);
  computeDraw.SetInt("height", height);

  computeDraw.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void LatticeGas::Randomize()
{
  // For a 512x512 RGBA8 texture
  std::vector<unsigned char> randomData(width * height * 4);

  for (int i = 0; i < randomData.size(); ++i)
  {
    randomData[i] = rand() % 2;
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
void LatticeGas::DrawCPU(float mouseX, float mouseY)
{
  glBindTexture(GL_TEXTURE_2D, m_DataTexture.id);

  int x = mouseX * width;
  int y = mouseY * height;

  unsigned char data[] = {direction == 1, direction == 2, direction == 3, direction == 4};

  glTexSubImage2D(GL_TEXTURE_2D,
                  0,                // Mipmap level
                  x, y,             // x and y offset
                  1, 1,             // Dimensions of the area to update
                  GL_RGBA_INTEGER,  // Format of the pixel data on CPU
                  GL_UNSIGNED_BYTE, // Data type of the pixel data
                  data              // Pointer to the actual data on the CPU
  );
}

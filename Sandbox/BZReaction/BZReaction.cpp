#include "BZReaction.h"
#include "computeShader.h"
#include "imgui.h"
#include "simulator.h"
#include <array>
#include <functional>

// Belousov–Zhabotinsky reaction

BZReaction::BZReaction() : Automaton()
{

  computeSims.push_back(ComputeShader("../Sandbox/BZReaction/sim.comp"));
  computeRender = ComputeShader("../Sandbox/BZReaction/render.comp");
  computeDraw   = ComputeShader("../Sandbox/BZReaction/draw.comp");

  label = "BZReaction";

  dynamicResize = true;

  simFPS = 25;

  uniformVars.push_back({"n", std::ref(n)});
  uniformVars.push_back({"k1", std::ref(k1)});
  uniformVars.push_back({"k2", std::ref(k2)});
  uniformVars.push_back({"g", std::ref(g)});

  saved["BZ"]     = {200, 3, 3, 28};
  saved["Spiral"] = {100, 2, 3, 20};
  saved["Blocks"] = {100, 2, 3, 25};
  saved["Idk"]    = {100, 2, 3, 30};

  LoadSettings(saved["BZ"]);
}

void BZReaction::LoadSettings(std::array<int, 4> arr)
{
  n  = arr[0];
  k1 = arr[1];
  k2 = arr[2];
  g  = arr[3];
}

void BZReaction::UI()
{
  ImGui::Begin("hemlo");

  ImGui::SliderFloat("Radius", &drawRadius, 1.0f, 300.0f);

  if (ImGui::Button("Randomize")) Randomize();

  ImGui::DragInt("n", &n, 0.1f);
  ImGui::DragInt("k1", &k1, 0.05f);
  ImGui::DragInt("k2", &k2, 0.05f);
  ImGui::DragInt("g", &g, 0.05f);

  for (const auto &pair : saved)
  {
    if (ImGui::Selectable(pair.first))
    {
      LoadSettings(pair.second);
    }
  }
  ImGui::End();
}

void BZReaction::Render()
{
  computeRender.Activate();

  m_DataTexture.Bind(0);
  m_RenderDataTexture.Bind(1);

  computeRender.SetInt("n", n);

  computeRender.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void BZReaction::OnDraw(float mouseX, float mouseY)
{
  DrawGPU(mouseX, mouseY);
}
void BZReaction::DrawGPU(float mouseX, float mouseY)
{
  computeDraw.Activate();
  computeDraw.SetVec2("u_MousePos", glm::vec2(mouseX, mouseY));

  computeDraw.SetInt("width", width);
  computeDraw.SetInt("height", height);
  computeDraw.SetFloat("radius", drawRadius);

  computeDraw.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void BZReaction::Randomize()
{
  // For a 512x512 RGBA8 texture
  std::vector<unsigned char> randomData(width * height * 4);

  for (int i = 0; i < randomData.size(); ++i)
  {
    randomData[i] = rand() % n;
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

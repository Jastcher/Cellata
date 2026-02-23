#include "sandsim.h"
#include "imgui.h"

Sandsim::Sandsim()
    : Automaton(), particles({{0, 0, "Air"}, {1, GRAIN | SOLID, "Sand"}, {2, SOLID, "Stone"}}),
      colors({glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)})
{
  colorBuffer.Init(colors.size() * sizeof(glm::vec4), colors.data());

  computeSim    = ComputeShader("../Sandbox/Sandsim/sim.comp");
  computeDraw   = ComputeShader("../Sandbox/Sandsim/draw.comp");
  computeRender = ComputeShader("../Sandbox/Sandsim/render.comp");

  label = "Sandsim";

  simFPS = 60.0f;

  dynamicResize = true;
}

void Sandsim::UI()
{
  ImGui::Begin("hemlo");

  ImGui::SliderFloat("radius", &drawRadius, 0.1f, 50.0f);

  for (int n = 0; n < (int)particles.size(); n++)
  {
    ImGui::PushID(n);
    ImGui::AlignTextToFramePadding();

    if (ImGui::Selectable("##selectable", selection == n, ImGuiSelectableFlags_AllowOverlap)) selection = n;

    ImGui::SameLine(0, 0);
    ImVec4 color = ImVec4(colors[n].x, colors[n].y, colors[n].z, 1.0f);
    ImGui::ColorButton("##color", color, ImGuiColorEditFlags_NoTooltip, ImVec2(50, 0));

    ImGui::SameLine();
    ImGui::Text("%s", particles[n].name);
    ImGui::PopID();
  }
  ImGui::End();
}

void Sandsim::OnDraw(float mouseX, float mouseY)
{
  DrawGPU(mouseX, mouseY);
}
void Sandsim::DrawGPU(float mouseX, float mouseY)
{
  computeDraw.Activate();
  m_DataTexture.Bind(0);

  computeDraw.SetVec2("u_MousePos", glm::vec2(mouseX, mouseY));
  computeDraw.SetFloat("radius", drawRadius);
  computeDraw.SetInt("id", particles[selection].id);
  computeDraw.SetInt("props", particles[selection].props);
  computeDraw.SetInt("width", width);
  computeDraw.SetInt("height", height);

  computeDraw.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Sandsim::Render()
{
  computeRender.Activate();

  m_DataTexture.Bind(0);
  m_RenderDataTexture.Bind(1);
  colorBuffer.Bind(2);

  computeRender.Dispatch(width / 8, height / 8);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

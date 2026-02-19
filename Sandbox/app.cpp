
#include "application.h"
#include "imgui.h"
#include "simulator.h"
#include "storageBuffer.h"
#include <execution>

class Sandsim : public Automaton
{
public:
  Sandsim() : Automaton()
  {
    colorBuffer.Init(colors.size() * sizeof(glm::vec4), colors.data());
  }

private:
  struct Particle
  {
    unsigned char id;
    unsigned char props;

    const char *name;
  };

  enum Properties
  {
    SOLID  = 1,
    LIQUID = 2,
    GRAIN  = 4,
  };

  std::vector<Particle> particles = {{0, 0, "Air"}, {1, GRAIN | SOLID, "Sand"}, {2, SOLID, "Stone"}};
  std::vector<glm::vec4> colors   = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
                                     glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)};

  StorageBuffer colorBuffer;

  int selection = 1;

  void UI() override
  {
    ImGui::Begin("hemlo");

    for (int n = 0; n < particles.size(); n++)
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
  void Draw(int mouseX, int mouseY) override
  {
    computeDraw.Activate();
    m_DataTexture.Bind(0);

    computeDraw.SetVec2("u_MousePos", glm::vec2(mouseX, mouseY));

    computeDraw.SetFloat("radius", drawRadius);

    computeDraw.SetInt("id", particles[selection].id);
    computeDraw.SetInt("props", particles[selection].props);

    computeDraw.Dispatch(m_Width / 8, m_Height / 8);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
  void Render() override
  {
    computeRender.Activate();

    m_DataTexture.Bind(0);
    m_RenderDataTexture.Bind(1);
    colorBuffer.Bind(2);

    computeRender.Dispatch(m_Width / 8, m_Height / 8);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }
};

int main()
{

  Cellata app;

  Sandsim sandsim;

  sandsim.computeDraw   = ComputeShader("../Sandbox/draw.comp");
  sandsim.computeSim    = ComputeShader("../Sandbox/sim.comp");
  sandsim.computeRender = ComputeShader("../Sandbox/render.comp");

  app.Add(&sandsim);
  app.Run();

  return 0;
}

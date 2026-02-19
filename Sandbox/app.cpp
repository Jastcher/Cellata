

#include "application.h"
#include "computeShader.h"
#include "simulator.h"

class Sandsim : public Automaton
{
  void UI() override
  {
    ImGui::Begin("hemlo");
    ImGui::Text("MUHAHAH");
    ImGui::End();
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

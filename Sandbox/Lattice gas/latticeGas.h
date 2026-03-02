#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"

class LatticeGas : public Automaton
{
public:
  LatticeGas();
  ~LatticeGas() = default;

  void UI() override;
  void DrawGPU(float mouseX, float mouseY) override;
  void DrawCPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;

  void Randomize();

  unsigned char direction = 0; // 0 = empty, 1 = up, 2 = right, 3 = down, 4 = left, 5 = wall

  bool gpuDraw = true;

private:
  bool selection = true;
};

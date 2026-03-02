#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"

class LangtonsAnt : public Automaton
{
public:
  LangtonsAnt();
  ~LangtonsAnt() = default;

  void UI() override;
  void DrawCPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;

private:
  bool selection = true;
};

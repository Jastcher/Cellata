#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"

class Wireworld : public Automaton
{
public:
  Wireworld();
  ~Wireworld() = default;

  void UI() override;
  void DrawCPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;

  std::array<const char *, 4> selectionNames = {"Empty", "Head", "Tail", "Conductor"};

  int selection = 0;
};

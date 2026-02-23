#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"

class Conway : public Automaton
{
public:
  Conway();
  ~Conway() = default;

  void UI() override;
  void DrawCPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;

private:
  bool selection = true;
};

#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"

class Mazectric : public Automaton
{
public:
  Mazectric();
  ~Mazectric() = default;

  void UI() override;
  void DrawCPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;

  void Randomize(int distrib);

private:
  bool selection = true;
};

#pragma once

// Belousov–Zhabotinsky reaction

#include <glm/glm.hpp>
#include <imgui.h>
#include <unordered_map>
#include "simulator.h"

class BZReaction : public Automaton
{
public:
  BZReaction();
  ~BZReaction() = default;

  void UI() override;
  void DrawGPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;
  void Render() override;

  void Randomize();
  void LoadSettings(std::array<int, 4> arr);

  int n, k1, k2, g;

  std::unordered_map<const char *, std::array<int, 4>> saved;

private:
  bool selection = true;
};

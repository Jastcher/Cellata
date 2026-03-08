#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"

class Mandelbrot : public Automaton
{
public:
  Mandelbrot();
  ~Mandelbrot() = default;

  void UI() override;
  void OnDraw(float mouseX, float mouseY) override;
  void Render() override;
  glm::vec4 GetPixelData(float mouseX, float mouseY);
  std::string GetTooltip(float mouseX, float mouseY) override;

  int maxIt = 100;

private:
  bool selection = true;
};

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include "simulator.h"
#include "storageBuffer.h"

class Sandsim : public Automaton
{
public:
  Sandsim();
  ~Sandsim() = default;

  void UI() override;
  void DrawGPU(float mouseX, float mouseY) override;
  void OnDraw(float mouseX, float mouseY) override;

  void Render() override;

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

  // Data Members
  std::vector<Particle> particles;
  std::vector<glm::vec4> colors;
  StorageBuffer colorBuffer;
  int selection    = 1;
  float drawRadius = 5.0f;
};

#include "simulator.h"
#include "glm/common.hpp"

Simulator::Simulator()
{
}

Simulator::~Simulator()
{
}

// resize every automata
void Simulator::Resize(int width, int height)
{

  for (auto &a : automata)
  {
    if (a->dynamicResize) a->Resize(width, height);
  }
}

void Simulator::Draw(float mouseX, float mouseY)
{
  automaton->OnDraw(mouseX, mouseY);
}

void Simulator::Step(float deltaTime)
{
  if (stepOnce)
  {
    automaton->Step();
    stepOnce = false;
  }
  // if not running, dont step
  if (!isSimRunning) return;

  timeAccumulator += deltaTime;

  double slice = 1.0f / automaton->simFPS;
  while (timeAccumulator >= slice)
  {
    automaton->Step();
    timeAccumulator -= slice;
  }
}

void Simulator::Render()
{
  automaton->Render();
}

void Simulator::AddAutomaton(Automaton *a)
{
  automata.push_back(a);
  automaton = a;
}

GLuint Simulator::GetRenderDataTextureID()
{
  return automaton->GetRenderDataTextureID();
}

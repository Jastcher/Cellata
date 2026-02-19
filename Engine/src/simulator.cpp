#include "simulator.h"

Simulator::Simulator(int width, int height) : m_Width(width), m_Height(height)
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
    a->Resize(width, height);
  }
  m_Width  = width;
  m_Height = height;
}

void Simulator::Draw(int mouseX, int mouseY)
{
  automaton->Draw(mouseX, mouseY);
}

void Simulator::Step()
{
  // if not running, dont step
  if (!isSimRunning) return;

  automaton->Step();
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

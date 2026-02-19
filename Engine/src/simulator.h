#pragma once

#include "automaton.h"
#include <GL/gl.h>
#include <vector>

class Simulator
{
public:
  Simulator(int width, int height);
  ~Simulator();

  void Draw(int mouseX, int mouseY);
  void Step();
  void Resize(int width, int height);
  void Render();

  GLuint GetRenderDataTextureID();

  void AddAutomaton(Automaton *automaton);

  bool isSimRunning = false;

  Automaton *automaton;
  std::vector<Automaton *> automata;

private:
  int m_Width, m_Height;
};

#pragma once

#include "automaton.h"
#include <GL/gl.h>
#include <vector>

class Simulator
{
public:
  Simulator();
  ~Simulator();

  void Draw(float mouseX, float mouseY);
  void Step(float deltaTime);
  void Resize(int width, int height);
  void Render();

  GLuint GetRenderDataTextureID();

  void AddAutomaton(Automaton *automaton);

  bool isSimRunning = false;
  bool stepOnce     = false;

  Automaton *automaton;
  std::vector<Automaton *> automata;

private:
  double timeAccumulator = 0.0;
};

#pragma once
#include "window.h"
#include "ui.h"
#include "simulator.h"

#include <memory>
class Cellata
{
public:
  Cellata();
  ~Cellata();

  void Run();

  bool Init();

  void Add(Automaton *automaton);

private:
  bool m_Running = false;

  std::shared_ptr<Window> m_Window;
  std::shared_ptr<UI> m_UI;
  std::shared_ptr<Simulator> m_Simulator;

  GLuint dataTexture;
};

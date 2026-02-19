#include "application.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include <iostream>
#include <memory>

Cellata::Cellata()
{
  Init();
};

Cellata::~Cellata() {};

bool Cellata::Init()
{

  m_Window    = std::make_shared<Window>();
  m_Simulator = std::make_shared<Simulator>(m_Window->GetWidth(), m_Window->GetHeight());
  m_UI        = std::make_shared<UI>(m_Window, m_Simulator);

  return true;
}

void Cellata::Add(Automaton *automaton)
{
  // has to resize
  automaton->Resize(m_UI->viewportSize.x, m_UI->viewportSize.y);
  m_Simulator->AddAutomaton(automaton);
}
void Cellata::Run()
{

  while (!glfwWindowShouldClose(m_Window->window))
  {

    if (m_UI->isMouseDown)
    {
      m_Simulator->Draw(m_UI->viewportMouseX, m_UI->viewportMouseY);
    }

    m_Simulator->Step();

    m_Simulator->Render();

    m_Window->Clear();

    m_UI->Render(m_Simulator->GetRenderDataTextureID());

    m_Window->Update();
  }
}

#pragma once
#include "window.h"
#include "simulator.h"
#include <memory>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class UI
{
public:
  UI(std::shared_ptr<Window> window, std::shared_ptr<Simulator> simulator);
  ~UI();

  void Render();

  bool viewportFocused;

  // viewport mouse coords
  float viewportMouseX, viewportMouseY;
  bool isMouseDown;

  bool didViewportResize = false;
  ImVec2 viewportSize;

  std::shared_ptr<Window> window;
  std::shared_ptr<Simulator> simulator;

  ImVec2 prevViewportSize;

  // camera things
  float zoom = 1.0f;
  ImVec2 scroll;
};

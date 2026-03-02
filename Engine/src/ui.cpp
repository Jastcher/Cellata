#include "ui.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <sched.h>
#include "imgui/imgui.h"
#include "imgui_internal.h"
#include "simulator.h"
static inline void StartFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

static inline void RenderUI()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UI::UI(std::shared_ptr<Window> _window, std::shared_ptr<Simulator> _simulator) : window(_window), simulator(_simulator)
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

  ImGui_ImplGlfw_InitForOpenGL(window->window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

UI::~UI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

static inline void MainWindow()
{
  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
  ImGuiWindowFlags host_window_flags = 0;
  host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
  host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  ImGui::Begin("MainWindow", nullptr, host_window_flags);
  ImGui::PopStyleVar(3);
  ImGuiID dockspace_id = ImGui::GetID("DockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
  ImGui::End();
}

static inline void ViewportWindow(UI *UI)
{
  ImGui::Begin("Viewport");

  // keybinds
  if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_Space))
  {
    UI->simulator->isSimRunning = !UI->simulator->isSimRunning;
    ImGui::SetKeyOwner(ImGuiKey_Space, ImGui::GetID("Viewport"));
  }
  if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_RightArrow))
  {
    UI->simulator->automaton->simFPS += 5;
    ImGui::SetKeyOwner(ImGuiKey_RightArrow, ImGui::GetID("Viewport"));
  }
  if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
  {

    UI->simulator->automaton->simFPS =
        std::clamp(UI->simulator->automaton->simFPS - 5, 0.0f, UI->simulator->automaton->simFPS);
    ImGui::SetKeyOwner(ImGuiKey_LeftArrow, ImGui::GetID("Viewport"));
  }

  // tab selection
  if (ImGui::BeginTabBar("Automata"))
  {
    for (auto &automaton : UI->simulator->automata)
    {
      if (ImGui::BeginTabItem(automaton->label))
      {
        UI->simulator->automaton = automaton;
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();
  }

  ImVec2 currentSize = ImGui::GetContentRegionAvail();

  // 2. RESIZE LOGIC
  if (currentSize.x != UI->prevViewportSize.x || currentSize.y != UI->prevViewportSize.y)
  {
    UI->didViewportResize = true;
    UI->prevViewportSize  = UI->viewportSize;
    UI->viewportSize      = currentSize;
    UI->simulator->Resize(currentSize.x, currentSize.y);
  }

  // 3. PAN & ZOOM INPUT HANDLING
  ImGuiIO &io = ImGui::GetIO();

  // We check window hover to allow zooming anywhere in the viewport
  if (ImGui::IsWindowHovered())
  {
    // Zooming logic
    if (io.MouseWheel != 0.0f)
    {
      float oldZoom = UI->zoom;
      UI->zoom += io.MouseWheel * 0.1f * UI->zoom;
      if (UI->zoom < 0.1f) UI->zoom = 0.1f; // Prevent inversion/too small

      // Zoom towards mouse: adjust scroll so the point under mouse stays fixed
      ImVec2 mousePosInCanvas =
          ImVec2(io.MousePos.x - ImGui::GetCursorScreenPos().x, io.MousePos.y - ImGui::GetCursorScreenPos().y);
      UI->scroll.x += (mousePosInCanvas.x / oldZoom) - (mousePosInCanvas.x / UI->zoom);
      UI->scroll.y += (mousePosInCanvas.y / oldZoom) - (mousePosInCanvas.y / UI->zoom);
    }

    // Panning logic
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f))
    {
      UI->scroll.x -= io.MouseDelta.x / UI->zoom;
      UI->scroll.y -= io.MouseDelta.y / UI->zoom;
    }
  }

  // 4. UV CALCULATION
  // Base flipped UVs: (0,1) to (1,0).
  // We apply scrolling (offset) and zoom (scale) to these ranges.
  float u0 = UI->scroll.x / currentSize.x;
  float v0 = 1.0f - (UI->scroll.y / currentSize.y);
  float u1 = u0 + (1.0f / UI->zoom);
  float v1 = v0 - (1.0f / UI->zoom);

  // 5. RENDER THE TEXTURE
  ImGui::Image((ImTextureID)(uintptr_t)UI->simulator->GetRenderDataTextureID(), currentSize, ImVec2(u0, v0),
               ImVec2(u1, v1));

  // 6. MOUSE INTERACTION & COORDINATE MAPPING
  UI->viewportFocused = ImGui::IsItemHovered();
  UI->isMouseDown     = (UI->viewportFocused && ImGui::IsMouseDown(ImGuiMouseButton_Left));

  if (UI->viewportFocused)
  {
    ImVec2 mouse    = ImGui::GetMousePos();
    ImVec2 imageMin = ImGui::GetItemRectMin(); // Top-left of the Image widget

    // 1. Get mouse pos relative to the widget (0.0 to 1.0)
    float relX = (mouse.x - imageMin.x) / currentSize.x;
    float relY = (mouse.y - imageMin.y) / currentSize.y;

    // 2. Map relative screen pos to the zoomed/panned texture coordinate
    UI->viewportMouseX = (relX / UI->zoom) + (UI->scroll.x / currentSize.x);

    float textureSpaceY = (relY / UI->zoom) + (UI->scroll.y / currentSize.y);
    UI->viewportMouseY  = 1.0f - textureSpaceY;
  }

  ImGui::End();
}

static inline void InfoWindow(UI *UI)
{
  ImGui::Begin("Info");
  ImGui::Text("Viewport mouse %f ; %f", UI->viewportMouseX, UI->viewportMouseY);
  ImGui::Text("Window size %d ; %d", UI->window->GetWidth(), UI->window->GetHeight());
  ImGui::Text("dt: %f", UI->window->deltaTime);
  ImGui::Text("fps: %f", 1.0f / UI->window->deltaTime);
  ImGui::Separator();
  ImGui::Text("Scroll: %f, %f", UI->scroll.x, UI->scroll.y);
  ImGui::Text("Zoom: %f", UI->zoom);
  ImGui::End();
}

static inline void ControlWindow(UI *UI)
{
  ImGui::Begin("Control");

  ImGui::Checkbox("Play", &UI->simulator->isSimRunning);
  if (ImGui::Button("Step")) UI->simulator->stepOnce = true;

  if (ImGui::Checkbox("Dynamic resize", &UI->simulator->automaton->dynamicResize))
    UI->simulator->Resize(UI->viewportSize.x, UI->viewportSize.y);

  auto &automaton = UI->simulator->automaton;
  int *res[2]     = {&automaton->width, &automaton->height};
  if (ImGui::InputInt2("Resolution", *res)) automaton->Resize(*res[0], *res[1]);

  ImGui::DragFloat("SimFPS", &automaton->simFPS, 1.0f, 0.0f, 600.0f);

  ImGui::End();
}

void UI::Render()
{
  StartFrame();

  ImGui::SetNextWindowSize(ImVec2(window->GetWidth(), window->GetHeight()));
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  // ImGui::ShowDemoWindow();

  MainWindow();
  ViewportWindow(this);
  ControlWindow(this);
  InfoWindow(this);
  // PropertiesWindow(this);
  simulator->automaton->UI();

  RenderUI();
}

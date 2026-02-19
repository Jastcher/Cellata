#include "ui.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include "imgui/imgui.h"
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
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

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

static inline void ViewportWindow(UI *UI, GLuint dataTexture)
{
  ImGui::Begin("Viewport");

  ImVec2 currentSize = ImGui::GetContentRegionAvail();

  if (currentSize.x != UI->prevViewportSize.x || currentSize.y != UI->prevViewportSize.y)
  {
    UI->didViewportResize = true;
    UI->prevViewportSize  = UI->viewportSize;
    UI->viewportSize      = currentSize;

    // Resize components
    UI->simulator->Resize(currentSize.x, currentSize.y);
  }

  ImGui::Image((ImTextureID)(uintptr_t)dataTexture, currentSize, ImVec2(0, 1), ImVec2(1, 0));

  UI->viewportFocused = ImGui::IsItemHovered();
  UI->isMouseDown     = false;

  if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
  {
    UI->isMouseDown = true;
  }

  ImVec2 mouse = ImGui::GetMousePos();

  // Get the actual position of the Image in screen space
  ImVec2 imageMin = ImGui::GetItemRectMin();
  ImVec2 imageMax = ImGui::GetItemRectMax();

  // Local mouse inside the image
  ImVec2 localMouse = ImVec2(mouse.x - imageMin.x, mouse.y - imageMin.y);

  // Flip Y for framebuffer coordinates
  UI->viewportMouseX = localMouse.x;
  UI->viewportMouseY = currentSize.y - localMouse.y;
  ImGui::End();
}

static inline void InfoWindow(UI *UI)
{
  ImGui::Begin("Info");
  ImGui::Text("Viewport mouse %d ; %d", UI->viewportMouseX, UI->viewportMouseY);
  ImGui::Text("Window size %d ; %d", UI->window->GetWidth(), UI->window->GetHeight());
  ImGui::End();
}

static inline void ControlWindow(UI *UI)
{
  ImGui::Begin("Control");

  ImGui::Checkbox("Play", &UI->simulator->isSimRunning);

  ImGui::End();
}

static inline void PropertiesWindow(UI *UI)
{
  Simulator *s = UI->simulator.get();

  ImGui::Begin("Properties");

  ImGui::End();
}

void UI::Render(GLuint dataTexture)
{
  StartFrame();

  ImGui::SetNextWindowSize(ImVec2(window->GetWidth(), window->GetHeight()));
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  // ImGui::ShowDemoWindow();

  MainWindow();
  ViewportWindow(this, dataTexture);
  ControlWindow(this);
  InfoWindow(this);
  // PropertiesWindow(this);
  simulator->automaton->UI();

  RenderUI();
}

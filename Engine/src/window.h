#pragma once
#include "glad/glad.h"
#include <chrono>
#include "GLFW/glfw3.h"

struct Props
{
  unsigned int width  = 800;
  unsigned int height = 600;
  const char *title   = "Sandbox";
};

struct UserPtr
{
  Props *props;
  bool isFrameResized = false;
};

class Window
{
public:
  Window();
  ~Window();

  bool Init();

  unsigned int GetWidth() const;
  unsigned int GetHeight() const;
  const char *GetTitle() const;

  double mouseX, mouseY;
  bool mouseClicked = false;

  void Clear();
  void Update();

  void CalculateTime();

  static void ErrorCallback(int error, const char *description);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);

  GLFWwindow *window;

  UserPtr userPtr;
  float deltaTime = 1 / 60.0;

private:
  Props m_Properties = Props();

  std::chrono::high_resolution_clock::time_point m_PrevTime;
};

#pragma once
#include "glad/glad.h"
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

  static void ErrorCallback(int error, const char *description);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height);

  GLFWwindow *window;

  UserPtr userPtr;

private:
  Props m_Properties = Props();
};

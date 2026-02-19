#include "window.h"
#include "GLFW/glfw3.h"
#include <iostream>

bool Window::Init()
{
  std::cout << "Initializing" << std::endl;
  if (!glfwInit())
    {
      std::cout << "glfw init failed" << std::endl;
      glfwTerminate();
      return 0;
    }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(m_Properties.width, m_Properties.height,
                            m_Properties.title, NULL, NULL);
  if (!window)
    {
      std::cout << "window failed to create" << std::endl;
      glfwTerminate();
      return 0;
    }

  glfwMakeContextCurrent(window);

  glfwSetErrorCallback(ErrorCallback);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "failed to init glad" << std::endl;
      return 0;
    }

  std::cout << glGetString(GL_VERSION) << std::endl;

  glViewport(0, 0, m_Properties.width, m_Properties.height);

  userPtr.props = &m_Properties;
  glfwSetWindowUserPointer(window, &userPtr);

  return 1;
}

Window::Window()
{
  Init();
}

Window::~Window()
{
  glfwDestroyWindow(window);
}

unsigned int Window::GetWidth() const
{
  return m_Properties.width;
}

unsigned int Window::GetHeight() const
{
  return m_Properties.height;
}

const char *Window::GetTitle() const
{
  return m_Properties.title;
}

void Window::Clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::ErrorCallback(int error, const char *description)
{
  std::cerr << "[ERROR] " << error << " -> " << description << std::endl;
}
void Window::FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{

  UserPtr &userPtr       = *(UserPtr *)glfwGetWindowUserPointer(window);
  userPtr.props->width   = width;
  userPtr.props->height  = height;
  userPtr.isFrameResized = true;

  glViewport(0, 0, width, height);
}

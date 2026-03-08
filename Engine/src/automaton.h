
#include "computeShader.h"
#include "dataTexture.h"
#include "window.h"
#include <functional>
#include <variant>

struct UniformVariable
{
  const char *name;

  std::variant<std::reference_wrapper<int>, std::reference_wrapper<float>, glm::vec2> data; // tagged union
};

class Automaton
{
public:
  Automaton();
  virtual ~Automaton();

  virtual void OnDraw(float mouseX, float mouseY);
  virtual void Step();
  virtual void UI();
  virtual void Render();
  virtual void Resize(int width, int height);
  virtual void DrawGPU(float mouseX, float mouseY);
  virtual void DrawCPU(float mouseX, float mouseY);
  virtual std::string GetTooltip(float mouseX, float mouseY);

  GLuint GetRenderDataTextureID();

  float drawRadius  = 10.0f;
  const char *label = "Automaton";

  std::vector<ComputeShader> computeSims; // Each element is for different layer of simulation
  ComputeShader computeDraw;              // For drawing with cursor onto the texture
  ComputeShader computeRender;            // For translating data into colors for final rendering

  bool dynamicResize = false;

  int width, height;

  bool tooltipEnable = false;

  float simFPS = 10.0f;

  std::vector<UniformVariable> uniformVars;

protected:
  DataTexture m_DataTexture;     // Current state of data
  DataTexture m_NextDataTexture; // Next state of data

  DataTexture m_RenderDataTexture; // Final state of data for rendering
};

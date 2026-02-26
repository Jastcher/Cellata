
#include "computeShader.h"
#include "dataTexture.h"

class Automaton
{
public:
  Automaton();
  ~Automaton();

  virtual void OnDraw(float mouseX, float mouseY);
  virtual void Step();
  virtual void UI();
  virtual void Render();
  virtual void Resize(int width, int height);
  virtual void DrawGPU(float mouseX, float mouseY);
  virtual void DrawCPU(float mouseX, float mouseY);

  GLuint GetRenderDataTextureID();

  float drawRadius  = 10.0f;
  const char *label = "Automaton";

  std::vector<ComputeShader> computeSims; // Each element is for different layer of simulation
  ComputeShader computeDraw;              // For drawing with cursor onto the texture
  ComputeShader computeRender;            // For translating data into colors for final rendering

  bool dynamicResize = false;

  int width, height;

  float simFPS = 10.0f;

protected:
  DataTexture m_DataTexture;     // Current state of data
  DataTexture m_NextDataTexture; // Next state of data

  DataTexture m_RenderDataTexture; // Final state of data for rendering
};

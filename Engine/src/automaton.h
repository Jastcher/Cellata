
#include "computeShader.h"
#include "dataTexture.h"

class Automaton
{
public:
  Automaton();
  ~Automaton();

  void Resize(int width, int height);

  virtual void Draw(int mouseX, int mouseY);
  virtual void Step();
  virtual void UI();
  virtual void Render();

  GLuint GetRenderDataTextureID();

  float drawRadius = 10.0f;

  ComputeShader computeSim;    // For simulating each pixel
  ComputeShader computeDraw;   // For drawing with cursor onto the texture
  ComputeShader computeRender; // For translating data into colors for final rendering

protected:
  DataTexture m_DataTexture;     // Current state of data
  DataTexture m_NextDataTexture; // Next state of data

  DataTexture m_RenderDataTexture; // Final state of data for rendering

  int m_Width, m_Height;
};

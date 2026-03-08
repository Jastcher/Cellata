#include "mandelbrot.h"
#include "computeShader.h"
#include "imgui.h"
#include "window.h"
#include <string>
#include <sstream>

Mandelbrot::Mandelbrot()
{
  m_DataTexture.format       = GL_RGBA32F;
  m_NextDataTexture.format   = GL_RGBA32F;
  m_RenderDataTexture.format = GL_RGBA8;

  computeSims.push_back(ComputeShader("../Sandbox/Mandelbrot/sim.comp"));
  computeRender = ComputeShader("../Sandbox/Mandelbrot/render.comp");

  label = "Mandelbrot";

  // Resize(16, 16);
  dynamicResize = true;
  tooltipEnable = true;

  uniformVars.push_back({"maxIt", std::ref(maxIt)});
}

void Mandelbrot::UI()
{
  ImGui::Begin("hemlo");

  ImGui::DragInt("max iters", &maxIt);

  ImGui::End();
}

void Mandelbrot::OnDraw(float mouseX, float mouseY)
{
}

void Mandelbrot::Render()
{
  computeRender.Activate();

  m_DataTexture.Bind(0);
  m_RenderDataTexture.Bind(1);

  computeRender.SetInt("maxIt", maxIt);

  computeRender.Dispatch(width, height);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

std::string Mandelbrot::GetTooltip(float mouseX, float mouseY)
{
  glm::vec4 data = GetPixelData(mouseX, mouseY);

  // Using stringstream for easy formatting
  std::stringstream ss;
  ss << "Coords: (" << data.x << ", " << data.y << ")\n";
  ss << "Iteration: " << (int)data.z;

  return ss.str();
}
glm::vec4 Mandelbrot::GetPixelData(float mouseX, float mouseY)
{
  glm::vec4 pixelData;

  int x = mouseX * width;
  int y = mouseY * height;

  glGetTextureSubImage(m_DataTexture.id,
                       0,        // Mipmap level
                       x, y, 0,  // x, y, z offsets
                       1, 1, 1,  // width, height, depth (1x1x1 pixel)
                       GL_RGBA,  // Format
                       GL_FLOAT, // Type (match your pixelData array)
                       sizeof(glm::vec4),
                       &pixelData[0] // Destination pointer
  );

  return pixelData;
}

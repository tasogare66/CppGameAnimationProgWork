#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Framebuffer {
public:
  bool init(uint32_t width, uint32_t height);
  bool resize(uint32_t newWidth, uint32_t newHeight);
  void bind();
  void unbind();
  void drawToScreen();
  void cleanup();
};
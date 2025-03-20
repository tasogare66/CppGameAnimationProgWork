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

private:
  uint32_t mBufferWidth = 640;
  uint32_t mBufferHeight = 480;
  GLuint mBuffer = 0;
  GLuint mColorTex = 0;
  GLuint mDepthBuffer = 0;

  bool checkComplete();
};
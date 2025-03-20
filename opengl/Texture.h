#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Texture {
public:
  bool loadTexture(const char* const textureFilename);
  void bind();
  void unbind();
  void cleanup();
};
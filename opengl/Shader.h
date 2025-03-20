#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Shader {
public:
  bool loadShaders(const char* const vertexShaderFileName, const char* const fragmentShaderFileName);
  void use();
  void cleanup();
};
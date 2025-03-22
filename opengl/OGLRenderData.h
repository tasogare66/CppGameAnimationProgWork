#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct OGLVertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 uv;
};

struct OGLMesh {
  std::vector<OGLVertex> vertices;
};

struct OGLRenderData {
  OGLRenderData() = default;
  OGLRenderData(GLFWwindow* window)
    : rdWindow(window)
  {}
  GLFWwindow* rdWindow = nullptr;

  int32_t rdWidth = 0;
  int32_t rdHeight = 0;

  uint32_t rdTriangleCount = 0;

  int32_t rdFieldOfView = 90;

  bool rdUseChangedShader = false;

  float rdFrameTime = 0.0f;
  float rdMatrixGenerateTime = 0.0f;
  float rdUploadToUBOTime = 0.0f;
  float rdUIGenerateTime = 0.0f;
  float rdUIDrawTime = 0.0f;
};

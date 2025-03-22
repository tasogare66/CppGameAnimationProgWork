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

  int rdWidth = 0;
  int rdHeight = 0;

  uint32_t rdTriangleCount = 0;

  double rdFrameTime = 0.0;
  float rdMatrixGenerateTime = 0.0f;
  float rdUploadToUBOTime = 0.0f;
  float rdUIGenerateTime = 0.0f;
  float rdUIDrawTime = 0.0f;
};

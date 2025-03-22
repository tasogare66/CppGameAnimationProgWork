#pragma once
#include <vector>
#include <string>
//#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Framebuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "UniformBuffer.h"

#include "OGLRenderData.h"

class OGLRenderer {
public:
  OGLRenderer(GLFWwindow* window);

  bool init(uint32_t width, uint32_t height);
  void setSize(uint32_t width, uint32_t height);
  void uploadData(const OGLMesh& vertexData);
  void draw();

  void handleKeyEvents(int key, int scancode, int action, int mods);

  void cleanup();

private:
  Shader mBasicShader{};
  Shader mChangedShader{};
  Framebuffer mFramebuffer{};
  Texture mTex{};
  VertexBuffer mVertexBuffer{};
  UniformBuffer mUniformBuffer{};

  GLFWwindow* mWindow = nullptr;
  int32_t mTriangleCount = 0;
  bool mUseChangedShader = false;
  int mWidth = 0;
  int mHeight = 0;

  /* create identity matrix by default */
  glm::mat4 mViewMatrix = glm::mat4(1.0f);
  glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
};

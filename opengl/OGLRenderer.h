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

#include "OGLRenderData.h"

class OGLRenderer {
public:
  bool init(uint32_t width, uint32_t height);
  void setSize(uint32_t width, uint32_t height);
  void uploadData(const OGLMesh& vertexData);
  void draw();
  void cleanup();

private:
  Shader mShader{};
  Framebuffer mFramebuffer{};
  Texture mTex{};
  VertexBuffer mVertexBuffer{};

  int32_t mTriangleCount = 0;
};

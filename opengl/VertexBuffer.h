#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OGLRenderData.h"

class VertexBuffer {
public:
  void init();
  void uploadData(const OGLMesh& vertexData);
  void bind();
  void unbind();
  void draw(GLuint mode, uint32_t start, uint32_t num);
  void cleanup();
private:
  GLuint mVAO = 0;
  GLuint mVertexVBO = 0;
};
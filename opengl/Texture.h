#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Texture {
public:
  bool loadTexture(const char* const textureFilename);
  void bind();
  void unbind();
  void cleanup();
private:
  GLuint mTexture = 0;
  int32_t mTexWidth = 0;
  int32_t mTexHeight = 0;
  int32_t mNumberOfChannels = 0;
  std::string mTextureName;
};
#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Shader {
public:
  bool loadShaders(const char* const vertexShaderFileName, const char* const fragmentShaderFileName);
  void use();
  void cleanup();
private:
  GLuint mShaderProgram = 0;

  bool createShaderProgram(const char* const vertexShaderFileName, const char* const fragmentShaderFileName);
  GLuint loadShader(const char* const shaderFileName, GLuint shaderType);
  std::string loadFileToString(const char* fileName);
  bool checkCompileStats(const char* shaderFileName, GLuint shader);
  bool checkLinkStats(const char* vertexShaderFileName, const char* fragmentShaderFileName, GLuint shaderProgram);
};
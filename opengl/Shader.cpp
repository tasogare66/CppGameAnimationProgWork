#include "Shader.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring> // strerror()

#include "Logger.h"

bool Shader::loadShaders(const char* const vertexShaderFileName, const char* const fragmentShaderFileName)
{
  Logger::log(1, "%s: loading vertex shader '%s' and fragment shader '%s'\n", __FUNCTION__, vertexShaderFileName, fragmentShaderFileName);

  if (!createShaderProgram(vertexShaderFileName, fragmentShaderFileName)) {
    Logger::log(1, "%s error: shader program creation failed\n", __FUNCTION__);
    return false;
  }

  return true;
}

void Shader::use()
{
  glUseProgram(mShaderProgram);
}

void Shader::cleanup()
{
  glDeleteProgram(mShaderProgram);
}

bool Shader::createShaderProgram(const char* const vertexShaderFileName, const char* const fragmentShaderFileName)
{
  GLuint vertexShader = loadShader(vertexShaderFileName, GL_VERTEX_SHADER);
  if (!vertexShader) {
    Logger::log(1, "%s: loading of vertex shader '%s' failed\n", __FUNCTION__, vertexShaderFileName);
    return false;
  }

  GLuint fragmentShader = loadShader(fragmentShaderFileName, GL_FRAGMENT_SHADER);
  if (!fragmentShader) {
    Logger::log(1, "%s: loading of fragment shader '%s' failed\n", __FUNCTION__, fragmentShaderFileName);
    return false;
  }

  mShaderProgram = glCreateProgram();

  glAttachShader(mShaderProgram, vertexShader);
  glAttachShader(mShaderProgram, fragmentShader);

  glLinkProgram(mShaderProgram);

  if (!checkLinkStats(vertexShaderFileName, fragmentShaderFileName, mShaderProgram)) {
    Logger::log(1, "%s error: program linking from vertex shader '%s' / fragment shader '%s' failed\n", __FUNCTION__, vertexShaderFileName, fragmentShaderFileName);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return false;
  }

  /* it is safe to delete the shaders here */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  Logger::log(1, "%s: shader program %#x successfully compiled from vertex shader '%s' and fragment shader '%s'\n", __FUNCTION__, mShaderProgram, vertexShaderFileName, fragmentShaderFileName);
  return true;
}

GLuint Shader::loadShader(const char* const shaderFileName, GLuint shaderType)
{
  std::string shaderAsText = loadFileToString(shaderFileName);
  Logger::log(4, "%s: loaded shader file '%s', size %i\n", __FUNCTION__, shaderFileName, shaderAsText.size());

  const char* shaderSource = shaderAsText.c_str();
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);
  glCompileShader(shader);

  if (!checkCompileStats(shaderFileName, shader)) {
    Logger::log(1, "%s error: compiling shader '%s' failed\n", __FUNCTION__, shaderFileName);
    return 0;
  }

  Logger::log(1, "%s: shader %#x loaded and compiled\n", __FUNCTION__, shader);
  return shader;
}

std::string Shader::loadFileToString(const char* fileName)
{
  std::ifstream inFile(fileName);
  std::string str;

  if (inFile.is_open()) {
    str.clear();
    // allocate string data (no slower realloc)
    inFile.seekg(0, std::ios::end);
    str.reserve(inFile.tellg());
    inFile.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(inFile)),
      std::istreambuf_iterator<char>());
    inFile.close();
  }
  else {
    Logger::log(1, "%s error: could not open file %s\n", __FUNCTION__, fileName);
    Logger::log(1, "%s error: system says '%s'\n", __FUNCTION__, strerror(errno));
    return std::string();
  }

  if (inFile.bad() || inFile.fail()) {
    Logger::log(1, "%s error: error while reading file %s\n", __FUNCTION__, fileName);
    inFile.close();
    return std::string();
  }

  inFile.close();
  Logger::log(1, "%s: file %s successfully read to string\n", __FUNCTION__, fileName);
  return str;
}

bool Shader::checkCompileStats(const char* shaderFileName, GLuint shader)
{
  GLint isShaderCompiled;
  int logMessageLength;
  std::vector<char> shaderLog;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
  if (!isShaderCompiled) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logMessageLength);
    shaderLog = std::vector<char>(logMessageLength + 1);
    glGetShaderInfoLog(shader, logMessageLength, &logMessageLength, shaderLog.data());
    shaderLog.at(logMessageLength) = '\0';
    Logger::log(1, "%s error: shader compile of shader '%s' failed\n", __FUNCTION__, shaderFileName);
    Logger::log(1, "%s compile log:\n%s\n", __FUNCTION__, shaderLog.data());
    return false;
  }

  return true;
}

bool Shader::checkLinkStats(const char* vertexShaderFileName, const char* fragmentShaderFileName, GLuint shaderProgram)
{
  GLint isProgramLinked;
  int logMessageLength;
  std::vector<char> programLog;

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isProgramLinked);
  if (!isProgramLinked) {
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logMessageLength);
    programLog = std::vector<char>(logMessageLength + 1);
    glGetProgramInfoLog(shaderProgram, logMessageLength, &logMessageLength, programLog.data());
    programLog.at(logMessageLength) = '\0';
    Logger::log(1, "%s error: program linking of shaders '%s' and '%s' failed\n", __FUNCTION__, vertexShaderFileName, fragmentShaderFileName);
    Logger::log(1, "%s compile log:\n%s\n", __FUNCTION__, programLog.data());
    return false;
  }

  return true;
}


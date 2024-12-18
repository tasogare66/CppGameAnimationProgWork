#pragma once
#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
  bool init(unsigned int width, unsigned int height, const char* const title);
  void mainLoop();
  void cleanup();

private:
  GLFWwindow* mWindow = nullptr;
};

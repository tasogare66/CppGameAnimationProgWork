#pragma once
#include <string>
#include <memory>

#include "OGLRenderer.h"

class Window {
public:
  bool init(unsigned int width, unsigned int height, const char* const title);
  void mainLoop();
  void cleanup();

private:
  void handleWindowMoveEvents(int xpos, int ypos);
  void handleWindowMinimizedEvents(int minimized);
  void handleWindowMaximizedEvents(int maximized);
  void handleWindowCloseEvents();

  void handleKeyEvents(int key, int scancode, int action, int mods);
  void handleMouseButtonEvents(int button, int action, int mods);
  void handleMousePositionEvents(double xpos, double ypos);
  void handleMouseEnterLeaveEvents(int enter);

  GLFWwindow* mWindow = nullptr;

  std::unique_ptr<OGLRenderer> mRenderer;
};

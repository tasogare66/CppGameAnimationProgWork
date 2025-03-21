#include "Window.h"
#include "Logger.h"

bool Window::init(unsigned int width, unsigned int height, const char* const title) {
  if (!glfwInit()) {
    Logger::log(1, "%s: glfwInit() error\n", __FUNCTION__);
    return false;
  }

  /* set a "hint" for the NEXT window created*/
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!mWindow) {
    Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(mWindow);

  mRenderer = std::make_unique<OGLRenderer>();
  if (!mRenderer->init(width, height)) {
    glfwTerminate();
    Logger::log(1, "%s error: Could not init OpenGL\n", __FUNCTION__);
    return false;
  }

  /* the C handlers needs a little 'stunt' here */
  /* 1) save the pointer to the instance as user pointer */
  glfwSetWindowUserPointer(mWindow, this);
  glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* win, int width, int height) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    auto renderer = thisWindow->mRenderer.get();
    renderer->setSize(width, height);
    }
  );

  glfwSetWindowPosCallback(mWindow, [](GLFWwindow* win, int xpos, int ypos) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleWindowMoveEvents(xpos, ypos);
    }
  );

  glfwSetWindowIconifyCallback(mWindow, [](GLFWwindow* win, int minimized) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleWindowMinimizedEvents(minimized);
    }
  );

  glfwSetWindowMaximizeCallback(mWindow, [](GLFWwindow* win, int maximized) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleWindowMaximizedEvents(maximized);
    }
  );

  glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* win) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleWindowCloseEvents();
    }
  );

  // key
  glfwSetKeyCallback(mWindow, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleKeyEvents(key, scancode, action, mods);
    }
  );

  // mouse
  glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* win, int button, int action, int mods) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleMouseButtonEvents(button, action, mods);
    }
  );

  glfwSetCursorPosCallback(mWindow, [](GLFWwindow* win, double xpos, double ypos) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleMousePositionEvents(xpos, ypos);
    }
  );

  glfwSetCursorEnterCallback(mWindow, [](GLFWwindow* win, int enter) {
    auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
    thisWindow->handleMouseEnterLeaveEvents(enter);
    }
  );

  mModel = std::make_unique<Model>();
  mModel->init();
  Logger::log(1, "%s: mockup model data loaded\n", __FUNCTION__);

  Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);
  return true;
}

void Window::handleWindowMoveEvents(int xpos, int ypos) {
  Logger::log(1, "%s: Window has been moved to %i/%i\n", __FUNCTION__, xpos, ypos);
}

void Window::handleWindowMinimizedEvents(int minimized) {
  if (minimized) {
    Logger::log(1, "%s: Window has been minimized\n", __FUNCTION__);
  }
  else {
    Logger::log(1, "%s: Window has been restored\n", __FUNCTION__);
  }
}

void Window::handleWindowMaximizedEvents(int maximized) {
  if (maximized) {
    Logger::log(1, "%s: Window has been maximized\n", __FUNCTION__);
  }
  else {
    Logger::log(1, "%s: Window has been restored\n", __FUNCTION__);
  }
}

void Window::handleWindowCloseEvents() {
  Logger::log(1, "%s: Window got close event... bye!\n", __FUNCTION__);
}

void Window::handleKeyEvents(int key, int scancode, int action, int mods) {
  const char* actionName = nullptr;
  switch (action) {
  case GLFW_PRESS:
    actionName = "pressed";
    break;
  case GLFW_RELEASE:
    actionName = "released";
    break;
  case GLFW_REPEAT:
    actionName = "repeated";
    break;
  default:
    actionName = "invalid";
    break;
  }
  const char* keyName = glfwGetKeyName(key, 0);
  Logger::log(1, "%s: key %s (key %i, scancode %i) %s\n", __FUNCTION__, keyName, key, scancode, actionName);
}

void Window::handleMouseButtonEvents(int button, int action, int mods) {
  const char* actionName = nullptr;
  switch (action) {
  case GLFW_PRESS:
    actionName = "pressed";
    break;
  case GLFW_RELEASE:
    actionName = "released";
    break;
  case GLFW_REPEAT:
    actionName = "repeated";
    break;
  default:
    actionName = "invalid";
    break;
  }

  const char* mouseButtonName = nullptr;
  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT:
    mouseButtonName = "left";
    break;
  case GLFW_MOUSE_BUTTON_MIDDLE:
    mouseButtonName = "middle";
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    mouseButtonName = "right";
    break;
  default:
    mouseButtonName = "other";
    break;
  }

  Logger::log(1, "%s: %s mouse button (%i) %s\n", __FUNCTION__, mouseButtonName, button, actionName);
}

void Window::handleMousePositionEvents(double xpos, double ypos) {
  Logger::log(1, "%s: Mouse is at position %lf/%lf\n", __FUNCTION__, xpos, ypos);
}

void Window::handleMouseEnterLeaveEvents(int enter) {
  if (enter) {
    Logger::log(1, "%s: Mouse entered window\n", __FUNCTION__);
  }
  else {
    Logger::log(1, "%s: Mouse left window\n", __FUNCTION__);
  }
}

void Window::mainLoop() {
  /* force VSYNC */
  glfwSwapInterval(1);

  /* upload only once for now */
  mRenderer->uploadData(mModel->getVertexData());

  while (!glfwWindowShouldClose(mWindow)) {
    mRenderer->draw();

    /* swap buffers */
    glfwSwapBuffers(mWindow);

    /* poll events in a loop */
    glfwPollEvents();
  }
}

void Window::cleanup() {
  mRenderer->cleanup();

  Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

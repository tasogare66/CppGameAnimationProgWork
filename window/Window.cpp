#include "Window.h"
#include "Logger.h"

bool Window::init(unsigned int width, unsigned int height, const char* const title) {
  if (!glfwInit()) {
    Logger::log(1, "%s: glfwInit() error\n", __FUNCTION__);
    return false;
  }

  /* set a "hint" for the NEXT window created*/
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (!mWindow) {
    Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(mWindow);

  /* the C handlers needs a little 'stunt' here */
  /* 1) save the pointer to the instance as user pointer */
  glfwSetWindowUserPointer(mWindow, this);

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

void Window::mainLoop() {
  /* force VSYNC */
  glfwSwapInterval(1);

  float color = 0.0f;
  while (!glfwWindowShouldClose(mWindow)) {
    color >= 1.0f ? color = 0.0f : color += 0.01f;

    glClearColor(color, color, color, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* swap buffers */
    glfwSwapBuffers(mWindow);

    /* poll events in a loop */
    glfwPollEvents();
  }
}

void Window::cleanup() {
  Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

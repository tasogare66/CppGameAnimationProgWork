#include "Window.h"
#include "Logger.h"

bool Window::init(unsigned int width, unsigned int height, std::string title) {
  Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);
  return true;
}

void Window::mainLoop() {

}

void Window::cleanup() {
  Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
}

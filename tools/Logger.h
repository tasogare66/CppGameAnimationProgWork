/* simple Logger class */
#pragma once
#include <cstdint>
#include <cstdio>

class Logger {
public:
  /* log if input log level is equal or smaller to log level set */
  template <typename... Args>
  static void log(uint32_t logLevel, Args ... args) {
    if (logLevel <= mLogLevel) {
      std::printf(args ...);
      /* force output, i.e. for Eclipse */
      std::fflush(stdout);
    }
  }

  static void setLogLevel(uint32_t inLogLevel) {
    mLogLevel = inLogLevel <= 9 ? inLogLevel : 9;
  }

private:
  static uint32_t mLogLevel;
};

#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& getLogger() {
    static Logger instance;
    return instance;
  }

  void log(const std::string& string, bool newLine = true);

 private:
  std::mutex mutex_;
  std::ofstream fileStream_;

  Logger();
  Logger(Logger&&) = delete;
  Logger(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
};
}  // namespace uni_course_cpp

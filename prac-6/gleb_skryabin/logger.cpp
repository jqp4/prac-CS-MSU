#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "config.hpp"

namespace {

std::string getCurrentDateTime() {
  std::stringstream dateTimeStringStream;
  const auto dateTime = std::chrono::system_clock::now();
  const auto dateTimeT = std::chrono::system_clock::to_time_t(dateTime);
  dateTimeStringStream << std::put_time(std::localtime(&dateTimeT),
                                        "%Y.%m.%d %H:%M:%S");

  return dateTimeStringStream.str();
}

}  // namespace

namespace uni_course_cpp {

void Logger::log(const std::string& string, bool newLine) {
  const std::lock_guard<std::mutex> lock(mutex_);
  const std::string output = getCurrentDateTime() + " " + string;
  fileStream_ << output;
  std::cout << output;

  if (newLine) {
    fileStream_ << std::endl;
    std::cout << std::endl;
  }
}

Logger::Logger() : fileStream_(config::kLogFilePath) {}

}  // namespace uni_course_cpp

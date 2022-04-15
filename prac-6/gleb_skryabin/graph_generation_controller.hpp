#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph_generator.hpp"

namespace uni_course_cpp {
class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenStartedCallback = std::function<void(int i)>;
  using GenFinishedCallback = std::function<void(int i, Graph graph)>;

  class Worker {
   public:
    using GetJobCallback = std::function<std::optional<JobCallback>()>;

    enum class State { Idle, Working, ShouldTerminate };

    explicit Worker(const GetJobCallback& getJobCallback)
        : getJobCallback_(getJobCallback) {}

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    GetJobCallback getJobCallback_;
    std::atomic<State> state_ = State::Idle;
  };

  GraphGenerationController(int threadsCount,
                            int graphsCount,
                            const GraphGenerator::Params& graphGeneratorParams);

  void generate(const GenStartedCallback& genStartedCallback,
                const GenFinishedCallback& genFinishedCallback);

 private:
  const int graphsCount_;
  const GraphGenerator graphGenerator_;

  std::mutex mutex_;
  std::mutex mutexStartedCallback_;
  std::mutex mutexFinishedCallback_;
  std::list<JobCallback> jobs_;
  std::list<Worker> workers_;
};
}  // namespace uni_course_cpp

#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <vector>
#include "graph_generator.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {
class GraphTraversalController {
 public:
  using TraversalStartedCallback = std::function<void(int index)>;
  using TraversalFinishedCallback =
      std::function<void(int index, std::vector<GraphPath> paths)>;

  class Worker {
   public:
    using GetJobCallback =
        std::function<std::optional<std::function<void()>>()>;

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

  void traverse(const TraversalStartedCallback& traversalStartedCallback,
                const TraversalFinishedCallback& traversalFinishedCallback);

  GraphTraversalController(const std::vector<Graph>& graphs);

 private:
  int threadsNum_;
  std::list<Worker> workers_;
  std::list<std::function<void()>> jobs_;
  std::atomic<int> graphsTraversed_ = 0;
  const std::vector<Graph>& graphs_;

  std::mutex mutexJobs_;
  std::mutex mutexStart_;
  std::mutex mutexFinish_;
};
}  // namespace uni_course_cpp

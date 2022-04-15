#include "graph_generation_controller.hpp"
#include <cassert>

namespace uni_course_cpp {
GraphGenerationController::GraphGenerationController(
    int threadsCount,
    int graphsCount,
    const GraphGenerator::Params& graphGeneratorParams)
    : graphsCount_(graphsCount), graphGenerator_(graphGeneratorParams) {
  for (int i = 0; i < threadsCount; i++) {
    workers_.emplace_back(
        [&jobs = jobs_, &mutex = mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard<std::mutex> lock(mutex);
          if (jobs.empty()) {
            return std::nullopt;
          }
          const auto job = jobs.front();
          jobs.pop_front();
          return job;
        });
  }
}

void GraphGenerationController::generate(
    const GenStartedCallback& generateStartedCallback,
    const GenFinishedCallback& generateFinishedCallback) {
  std::atomic<int> jobsCounter = 0;

  for (int index = 0; index < graphsCount_; index++) {
    jobs_.emplace_back([index, &graphGenerator = graphGenerator_,
                        &mutexStartedCallback = mutexStartedCallback_,
                        &mutexFinishedCallback = mutexFinishedCallback_,
                        &generateStartedCallback, &generateFinishedCallback,
                        &jobsCounter]() {
      {
        const std::lock_guard<std::mutex> lock(mutexStartedCallback);
        generateStartedCallback(index);
      }
      auto graph = graphGenerator.generate();
      {
        const std::lock_guard<std::mutex> lock(mutexFinishedCallback);
        generateFinishedCallback(index, std::move(graph));
      }

      jobsCounter++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobsCounter < graphsCount_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
  assert(state_ == State::Idle);
  state_ = State::Working;
  thread_ =
      std::thread([&state_ = state_, &getJobCallback_ = getJobCallback_]() {
        while (true) {
          if (state_ == State::ShouldTerminate) {
            state_ = State::Idle;
            return;
          }

          const auto jobOptional = getJobCallback_();
          if (jobOptional.has_value()) {
            const auto job_callback = jobOptional.value();
            job_callback();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_course_cpp

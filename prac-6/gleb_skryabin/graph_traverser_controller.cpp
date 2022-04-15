#include "graph_traverser_controller.hpp"
#include <algorithm>
#include <cassert>
#include <mutex>
#include <thread>
#include "graph.hpp"
#include "graph_path.hpp"
#include "graph_traverser.hpp"

namespace {

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

}  // namespace

namespace uni_course_cpp {

GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  threadsNum_ = std::min(MAX_WORKERS_COUNT, static_cast<int>(graphs.size()));
  for (int i = 0; i < threadsNum_; i++) {
    workers_.emplace_back(
        [&jobs = jobs_, this]() -> std::optional<std::function<void()>> {
          const std::lock_guard<std::mutex> lock(mutexJobs_);
          if (jobs.empty()) {
            return std::nullopt;
          }

          const auto job = jobs.front();
          jobs.pop_front();
          return job;
        });
  }
}

void GraphTraversalController::traverse(
    const TraversalStartedCallback& traversalStartedCallback,
    const TraversalFinishedCallback& traversalFinishedCallback) {
  for (int index = 0; index < graphs_.size(); index++) {
    jobs_.emplace_back([&mutexStart = mutexStart_,
                        &graphsTraversed = graphsTraversed_,
                        &traversalStartedCallback, &traversalFinishedCallback,
                        &mutexFinish = mutexFinish_, index, this]() {
      {
        const std::lock_guard<std::mutex> lock(mutexStart);
        traversalStartedCallback(index);
      }
      GraphTraverser graphTraverser(graphs_[index]);
      auto paths = graphTraverser.findAllPaths();
      {
        const std::lock_guard<std::mutex> lock(mutexFinish);
        traversalFinishedCallback(index, std::move(paths));
      }

      graphsTraversed++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (graphsTraversed_ != graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraversalController::Worker::start() {
  assert(state_ == State::Idle);
  state_ = State::Working;

  thread_ =
      std::thread([&state = state_, &getJobCallback = getJobCallback_, this]() {
        while (true) {
          if (state == State::ShouldTerminate) {
            state = State::Idle;
            return;
          }

          const auto jobOptional = getJobCallback();
          if (jobOptional.has_value()) {
            const auto& job = jobOptional.value();
            job();
          }
        }
      });
}

void GraphTraversalController::Worker::stop() {
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_course_cpp

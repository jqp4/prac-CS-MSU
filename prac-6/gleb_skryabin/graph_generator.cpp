#include "graph_generator.hpp"
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <random>
#include <stdexcept>
#include <thread>
#include <vector>
#include "graph.hpp"

namespace {

const int kMaxThreadCount = std::thread::hardware_concurrency();

bool itHappened(float probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_number(0, 100);
  return probability >= float(random_number(gen)) / 100;
}

uni_course_cpp::VertexId getRandomVertexId(
    const std::unordered_set<uni_course_cpp::VertexId>& vertexIds) {
  if (vertexIds.empty()) {
    return uni_course_cpp::INVALID_ID;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_number(0, vertexIds.size() - 1);
  auto it = std::begin(vertexIds);
  std::advance(it, random_number(gen));
  return *it;
}

std::unordered_set<uni_course_cpp::VertexId> getUnconnectedVertexIds(
    const uni_course_cpp::Graph& graph,
    const uni_course_cpp::VertexId& exclusiveVertexId,
    const std::unordered_set<uni_course_cpp::VertexId>& vertexIds,
    std::mutex& mutex) {
  std::unordered_set<uni_course_cpp::VertexId> newVertexIds;
  for (const auto& vertexId : vertexIds) {
    const bool isConnected = [&graph, &exclusiveVertexId, &vertexId, &mutex]() {
      const std::lock_guard<std::mutex> lock(mutex);
      return graph.checkConnection(exclusiveVertexId, vertexId);
    }();

    if (!isConnected) {
      newVertexIds.insert(vertexId);
    }
  }

  return newVertexIds;
}

}  // namespace

namespace uni_course_cpp {

Graph GraphGenerator::generate() const {
  Graph graph = Graph();
  const VertexId vertexRootId = graph.addVertex();

  std::mutex mutex;
  generateGrayEdges(graph, vertexRootId);
  std::thread greenThread(
      [&graph, &mutex, this]() { generateGreenEdges(graph, mutex); });
  std::thread yellowThread(
      [&graph, &mutex, this]() { generateYellowEdges(graph, mutex); });
  std::thread redThread(
      [&graph, &mutex, this]() { generateRedEdges(graph, mutex); });

  greenThread.join();
  yellowThread.join();
  redThread.join();
  return graph;
}

float GraphGenerator::getColorProbability(const Graph::Edge::Color& color,
                                          const Graph::Depth& depth = 0) const {
  switch (color) {
    case Graph::Edge::Color::Gray:
      return 1.0 - float(depth) / params_.getDepth();
    case Graph::Edge::Color::Green:
      return 0.1;
    case Graph::Edge::Color::Yellow:
      return float(depth) / (params_.getDepth() - 1);
    case Graph::Edge::Color::Red:
      return 0.33;
    default:
      throw std::runtime_error("Invalid Edge::Colors value");
  }
}

void GraphGenerator::generateGrayEdges(Graph& graph,
                                       VertexId vertexSourceId) const {
  std::mutex mutex;
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::atomic<bool> checkTermination = false;
  std::atomic<int> jobsDone = 0;

  for (int jobNumber = 0; jobNumber < params_.getNewVerticesCount();
       jobNumber++) {
    jobs.push_back([&jobsDone, &mutex, &graph, this]() {
      generateGrayBranch(graph, 0, 0, mutex);
      jobsDone++;
    });
  }

  const auto worker = [&checkTermination, &mutex, &jobs]() {
    while (true) {
      if (checkTermination) {
        return;
      }

      const auto optionalJob = [&jobs, &mutex]() -> std::optional<JobCallback> {
        const std::lock_guard<std::mutex> lock(mutex);
        if (jobs.size() > 0) {
          const auto job = jobs.back();
          jobs.pop_back();
          return job;
        }
        return std::nullopt;
      }();

      if (optionalJob.has_value()) {
        const auto& job = optionalJob.value();
        job();
      }
    }
  };

  const auto threadCount =
      std::min(kMaxThreadCount, params_.getNewVerticesCount());

  auto threads = std::vector<std::thread>();
  threads.reserve(threadCount);
  for (int threadNumber = 0; threadNumber < threadCount; ++threadNumber) {
    threads.push_back(std::thread(worker));
  }
  while (jobsDone < params_.getNewVerticesCount()) {
  }

  checkTermination = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generateGrayBranch(Graph& graph,
                                        Graph::Depth depth,
                                        const VertexId& vertexId,
                                        std::mutex& mutex) const {
  if (depth == params_.getDepth()) {
    return;
  }
  if (!itHappened(getColorProbability(Graph::Edge::Color::Gray, depth))) {
    return;
  }

  const VertexId& newVertexId = [&graph, &mutex, &vertexId]() {
    const std::lock_guard<std::mutex> lock(mutex);
    const VertexId& newVertexId = graph.addVertex();
    graph.addEdge(vertexId, newVertexId);
    return newVertexId;
  }();

  for (int jobNumber = 0; jobNumber < params_.getNewVerticesCount();
       jobNumber++) {
    generateGrayBranch(graph, depth + 1, newVertexId, mutex);
  }
}

void GraphGenerator::generateGreenEdges(Graph& graph, std::mutex& mutex) const {
  const float prob = getColorProbability(Graph::Edge::Color::Green);
  for (const auto& [vertexId, vertex] : graph.getVertices()) {
    if (itHappened(prob)) {
      const std::lock_guard<std::mutex> lock(mutex);
      graph.addEdge(vertexId, vertexId);
    }
  }
}

void GraphGenerator::generateYellowEdges(Graph& graph,
                                         std::mutex& mutex) const {
  for (Graph::Depth depth = 1; depth < graph.getDepth(); depth++) {
    const float prob = getColorProbability(Graph::Edge::Color::Yellow, depth);
    for (const VertexId& vertexSourceId : graph.getVertexIdsByDepth(depth)) {
      if (itHappened(prob)) {
        const auto& vertexTargetIds = graph.getVertexIdsByDepth(depth + 1);
        const auto unconnectedVertexIds = getUnconnectedVertexIds(
            graph, vertexSourceId, vertexTargetIds, mutex);

        const VertexId vertexTargetId = getRandomVertexId(unconnectedVertexIds);
        if (vertexTargetId != INVALID_ID) {
          const std::lock_guard<std::mutex> lock(mutex);
          graph.addEdge(vertexSourceId, vertexTargetId);
        }
      }
    }
  }
}

void GraphGenerator::generateRedEdges(Graph& graph, std::mutex& mutex) const {
  const float prob = getColorProbability(Graph::Edge::Color::Red);
  for (Graph::Depth depth = 0; depth < graph.getDepth() - 2; depth++) {
    for (const VertexId& vertexSourceId : graph.getVertexIdsByDepth(depth)) {
      if (itHappened(prob)) {
        const auto& vertexTargetIds = graph.getVertexIdsByDepth(depth + 2);
        const VertexId vertexTargetId = getRandomVertexId(vertexTargetIds);
        const std::lock_guard<std::mutex> lock(mutex);
        graph.addEdge(vertexSourceId, vertexTargetId);
      }
    }
  }
}

}  // namespace uni_course_cpp

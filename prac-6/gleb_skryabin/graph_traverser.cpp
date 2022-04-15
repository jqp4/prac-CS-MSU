#include "graph_traverser.hpp"
#include <atomic>
#include <cassert>
#include <climits>
#include <functional>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"

namespace {

using JobCallback = std::function<void()>;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();
constexpr uni_course_cpp::Graph::Edge::Duration MAX_DURATION = INT_MAX;
constexpr uni_course_cpp::GraphPath::Distance MAX_DISTANCE = INT_MAX;
constexpr uni_course_cpp::VertexId START_VERTEX_ID = 0;

}  // namespace

namespace uni_course_cpp {

GraphPath GraphTraverser::findShortestPath(
    const VertexId& vertexSourceId,
    const VertexId& vertexTargetId) const {
  std::unordered_map<VertexId, std::vector<EdgeId>> pathsInEdgeIds;
  std::unordered_map<VertexId, std::vector<VertexId>> pathsInVertexIds;
  std::unordered_map<VertexId, Graph::Edge::Duration> durations;
  std::unordered_map<VertexId, GraphPath::Distance> distances;
  for (const auto& [vertexId, _] : graph_.getVertices()) {
    distances[vertexId] = MAX_DISTANCE;
  }

  distances[vertexSourceId] = 0;
  pathsInVertexIds[vertexSourceId].push_back(vertexSourceId);
  std::queue<VertexId> passWaiting;
  passWaiting.push(vertexSourceId);

  while (!passWaiting.empty()) {
    const VertexId currentVertexId = passWaiting.front();
    passWaiting.pop();
    for (const EdgeId& edgeId : graph_.getConnectedEdgeIds(currentVertexId)) {
      const Graph::Edge& edge = graph_.getEdges().at(edgeId);
      const VertexId nextVertexId =
          edge.vertexIds.first + edge.vertexIds.second - currentVertexId;

      if (distances[currentVertexId] + 1 < distances[nextVertexId]) {
        passWaiting.push(nextVertexId);
        durations[nextVertexId] = durations[nextVertexId];
        distances[nextVertexId] = distances[currentVertexId] + 1;
        pathsInVertexIds[nextVertexId] = pathsInVertexIds[currentVertexId];
        pathsInEdgeIds[nextVertexId] = pathsInEdgeIds[currentVertexId];
        pathsInVertexIds[nextVertexId].push_back(nextVertexId);
        pathsInEdgeIds[nextVertexId].push_back(edgeId);
        durations[nextVertexId] += edge.duration;
      }
    }
  }

  GraphPath shortestPath(durations[vertexTargetId],
                         std::move(pathsInVertexIds[vertexTargetId]),
                         std::move(pathsInEdgeIds[vertexTargetId]));

  return shortestPath;
}

GraphPath GraphTraverser::findFastestPath(
    const VertexId& vertexSourceId,
    const VertexId& vertexTargetId) const {
  std::unordered_map<VertexId, std::vector<EdgeId>> pathsInEdgeIds;
  std::unordered_map<VertexId, std::vector<VertexId>> pathsInVertexIds;
  std::unordered_map<VertexId, Graph::Edge::Duration> path_durations;
  std::unordered_map<VertexId, Graph::Edge::Duration> durations;
  for (const auto& [vertexId, _] : graph_.getVertices()) {
    durations[vertexId] = MAX_DURATION;
  }

  durations[vertexSourceId] = 0;
  pathsInVertexIds[vertexSourceId].push_back(vertexSourceId);
  std::queue<VertexId> pass_waiting;
  pass_waiting.push(vertexSourceId);

  while (!pass_waiting.empty()) {
    const auto currentVertexId = pass_waiting.front();
    pass_waiting.pop();
    for (const auto& edgeId : graph_.getConnectedEdgeIds(currentVertexId)) {
      const Graph::Edge& edge = graph_.getEdges().at(edgeId);
      const VertexId nextVertexId =
          edge.vertexIds.first + edge.vertexIds.second - currentVertexId;

      if (durations[currentVertexId] + edge.duration <
          durations[nextVertexId]) {
        pass_waiting.push(nextVertexId);
        durations[nextVertexId] = durations[currentVertexId] + edge.duration;
        pathsInVertexIds[nextVertexId] = pathsInVertexIds[currentVertexId];
        pathsInEdgeIds[nextVertexId] = pathsInEdgeIds[currentVertexId];
        path_durations[nextVertexId] = path_durations[currentVertexId];

        pathsInVertexIds[nextVertexId].push_back(nextVertexId);
        pathsInEdgeIds[nextVertexId].push_back(edgeId);
        path_durations[nextVertexId] += edge.duration;
      }
    }
  }

  return GraphPath(durations[vertexTargetId],
                   std::move(pathsInVertexIds[vertexTargetId]),
                   std::move(pathsInEdgeIds[vertexTargetId]));
}

std::vector<GraphPath> GraphTraverser::findAllPaths() const {
  std::mutex mutex;
  std::vector<GraphPath> paths;
  auto jobs = std::list<JobCallback>();
  std::atomic<int> finishedPathsCount = 0;
  std::atomic<bool> shouldTerminate = false;
  auto lastDepthLayer = graph_.getVertexIdsByDepth(graph_.getDepth());

  for (const VertexId& endVertexId : lastDepthLayer) {
    jobs.push_back([endVertexId, &paths, &mutex, &finishedPathsCount, this]() {
      const GraphPath newPath = findShortestPath(START_VERTEX_ID, endVertexId);
      {
        const std::lock_guard<std::mutex> lock(mutex);
        paths.push_back(std::move(newPath));
      }
      finishedPathsCount++;
    });
  }

  const auto worker = [&shouldTerminate, &mutex, &jobs]() {
    while (true) {
      if (shouldTerminate) {
        return;
      }

      const auto job_optional = [&jobs,
                                 &mutex]() -> std::optional<JobCallback> {
        const std::lock_guard lock(mutex);
        if (jobs.empty()) {
          return std::nullopt;
        }

        const auto job = jobs.front();
        jobs.pop_front();
        return job;
      }();

      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  const auto threadsNumber =
      std::min(MAX_THREADS_COUNT, static_cast<int>(lastDepthLayer.size()));

  auto threads = std::vector<std::thread>();
  threads.reserve(threadsNumber);

  for (int i = 0; i < threadsNumber; i++) {
    threads.push_back(std::thread(worker));
  }

  while (finishedPathsCount < lastDepthLayer.size()) {
  }

  shouldTerminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
  return paths;
}

}  // namespace uni_course_cpp

#include "graph.hpp"
#include <cassert>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

int getRandomInt(int start, int end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(start, end);
  return distrib(gen);
}

using Color = uni_course_cpp::Graph::Edge::Color;

uni_course_cpp::Graph::Edge::Duration calculateEdgeDuration(
    const Color& color) {
  switch (color) {
    case Color::Gray:
      return getRandomInt(1, 2);
    case Color::Green:
      return getRandomInt(1, 2);
    case Color::Yellow:
      return getRandomInt(1, 3);
    case Color::Red:
      return getRandomInt(2, 4);
  }
  throw std::runtime_error("Invalid color");
}

}  // namespace

namespace uni_course_cpp {

Graph::Edge::Edge(const EdgeId& _id,
                  const VertexId& vertexSourceId,
                  const VertexId& vertexTargetId,
                  const Graph::Edge::Color& _color)
    : id(_id),
      color(_color),
      vertexIds(vertexSourceId, vertexTargetId),
      duration(calculateEdgeDuration(_color)) {}

VertexId Graph::addVertex() {
  const VertexId newVertexId = generateVertexId();
  connectivityList_.emplace(newVertexId, std::unordered_set<EdgeId>());
  const Vertex newVertex = Vertex(newVertexId);
  vertices_.emplace(newVertexId, newVertex);
  verticesDepths_.emplace(newVertexId, 0);

  if (depthMap_.size() == 0) {
    const std::unordered_set<VertexId> newDepthLayer = {};
    depthMap_.push_back(newDepthLayer);
  }

  depthMap_.at(0).insert(newVertexId);
  return newVertexId;
}

EdgeId Graph::addEdge(const VertexId& vertexSourceId,
                      const VertexId& vertexTargetId) {
  assert(isContainVertex(vertexSourceId) && "ERROR: Vertex doesn't exists");
  assert(isContainVertex(vertexTargetId) && "ERROR: vertex doesn't exists");
  assert(!checkConnection(vertexSourceId, vertexTargetId) &&
         "ERROR: edge already exists");

  const EdgeId newEdgeId = generateEdgeId();
  const Edge::Color color = identifyEdgeColor(vertexSourceId, vertexTargetId);
  edges_.emplace(newEdgeId,
                 Edge(newEdgeId, vertexSourceId, vertexTargetId, color));

  if (color == Edge::Color::Gray) {
    updateVertexDepth(vertexTargetId, getVertexDepth(vertexSourceId) + 1);
  }

  coloredEdges_[color].insert(newEdgeId);
  connectivityList_.at(vertexSourceId).insert(newEdgeId);
  if (vertexSourceId != vertexTargetId) {
    connectivityList_.at(vertexTargetId).insert(newEdgeId);
  }
  return newEdgeId;
}

bool Graph::checkConnection(const VertexId& vertexSourceId,
                            const VertexId& vertexTargetId) const {
  assert(isContainVertex(vertexSourceId) && "ERROR: Vertex doesn't exists");
  assert(isContainVertex(vertexTargetId) && "ERROR: vertex doesn't exists");

  if (vertexSourceId != vertexTargetId) {
    for (const auto& sourceEdgeId : connectivityList_.at(vertexSourceId)) {
      for (const auto& targetEdgeId : connectivityList_.at(vertexTargetId)) {
        if (sourceEdgeId == targetEdgeId) {
          return true;
        }
      }
    }
  } else {
    for (const auto& edgeId : connectivityList_.at(vertexSourceId)) {
      const auto& connnectedVertexIds = edges_.at(edgeId).vertexIds;
      if (connnectedVertexIds.first == connnectedVertexIds.second) {
        return true;
      }
    }
  }

  return false;
}

Graph::Depth Graph::getVertexDepth(const VertexId& vertexId) const {
  assert(isContainVertex(vertexId) && "ERROR: Vertex doesn't exists");
  return verticesDepths_.at(vertexId);
}

Graph::Depth Graph::getDepth() const {
  return depthMap_.size() - 1;
}

const std::unordered_map<EdgeId, Graph::Edge>& Graph::getEdges() const {
  return edges_;
}

const std::unordered_map<VertexId, Graph::Vertex>& Graph::getVertices() const {
  return vertices_;
}

const std::unordered_set<VertexId>& Graph::getVertexIdsByDepth(
    Depth depth) const {
  return depthMap_.at(depth);
}

const std::unordered_set<EdgeId>& Graph::getEdgeIdsByColor(
    const Edge::Color& color) const {
  if (coloredEdges_.find(color) == coloredEdges_.end()) {
    static const std::unordered_set<EdgeId> empty_result = {};
    return empty_result;
  }

  return coloredEdges_.at(color);
}

const std::unordered_set<EdgeId>& Graph::getConnectedEdgeIds(
    const VertexId& vertexId) const {
  return connectivityList_.at(vertexId);
}

Graph::Edge::Color Graph::identifyEdgeColor(const VertexId& vertexSourceId,
                                            const VertexId& vertexTargetId) {
  if (vertexSourceId == vertexTargetId) {
    return Edge::Color::Green;
  }

  if (connectivityList_.at(vertexTargetId).size() == 0) {
    return Edge::Color::Gray;
  }

  const Depth vertexSourceDepth = getVertexDepth(vertexSourceId);
  const Depth vertexTargetDepth = getVertexDepth(vertexTargetId);
  if (vertexTargetDepth - vertexSourceDepth == 1 &&
      !checkConnection(vertexSourceId, vertexTargetId)) {
    return Edge::Color::Yellow;
  }

  if (vertexTargetDepth - vertexSourceDepth == 2) {
    return Edge::Color::Red;
  }

  throw std::runtime_error("Failed to determine color");
}

void Graph::updateVertexDepth(const VertexId& vertexId, const Depth& depth) {
  depthMap_.at(verticesDepths_.at(vertexId)).erase(vertexId);
  verticesDepths_.at(vertexId) = depth;

  if (depthMap_.size() <= depth) {
    const std::unordered_set<VertexId> newDepthLayer = {};
    depthMap_.push_back(newDepthLayer);
  }

  depthMap_.at(depth).insert(vertexId);
}

bool Graph::isContainVertex(const VertexId& vertexId) const {
  return vertices_.find(vertexId) != vertices_.end();
}

EdgeId Graph::generateEdgeId() {
  return nextEdgeId_++;
}
VertexId Graph::generateVertexId() {
  return nextVertexId_++;
}

}  // namespace uni_course_cpp

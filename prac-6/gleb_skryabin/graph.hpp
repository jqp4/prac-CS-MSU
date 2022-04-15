#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace uni_course_cpp {

using EdgeId = int;
using VertexId = int;
constexpr int INVALID_ID = -1;

class Graph {
 public:
  using Depth = int;

  struct Vertex {
   public:
    explicit Vertex(const VertexId& _id) : id(_id) {}
    const VertexId id = INVALID_ID;
  };

  struct Edge {
   public:
    using Duration = int;
    enum class Color { Gray, Green, Yellow, Red };

    Edge(const EdgeId& _id,
         const VertexId& vertexSourceId,
         const VertexId& vertexTargetId,
         const Color& _color);

    const Duration duration = 0;
    const EdgeId id = INVALID_ID;
    const Color color = Color::Gray;
    const std::pair<VertexId, VertexId> vertexIds = {INVALID_ID, INVALID_ID};
  };

  VertexId addVertex();

  EdgeId addEdge(const VertexId& vertexSourceId,
                 const VertexId& vertexTargetId);

  bool checkConnection(const VertexId& vertexSourceId,
                       const VertexId& vertexTargetId) const;

  Depth getDepth() const;
  Depth getVertexDepth(const VertexId& vertexId) const;

  const std::unordered_map<EdgeId, Edge>& getEdges() const;
  const std::unordered_map<VertexId, Vertex>& getVertices() const;
  const std::unordered_set<VertexId>& getVertexIdsByDepth(Depth depth) const;
  const std::unordered_set<EdgeId>& getEdgeIdsByColor(
      const Edge::Color& color) const;

  const std::unordered_set<EdgeId>& getConnectedEdgeIds(
      const VertexId& vertexId) const;

 private:
  bool isContainVertex(const VertexId& vertexId) const;
  void updateVertexDepth(const VertexId& vertexId, const Depth& depth);
  Edge::Color identifyEdgeColor(const VertexId& vertexSourceId,
                                const VertexId& vertexTargetId);

  EdgeId nextEdgeId_ = 0;
  VertexId nextVertexId_ = 0;
  EdgeId generateEdgeId();
  VertexId generateVertexId();

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<VertexId, Depth> verticesDepths_;
  std::unordered_map<VertexId, std::unordered_set<EdgeId>> connectivityList_;
  std::unordered_map<Edge::Color, std::unordered_set<EdgeId>> coloredEdges_;
  std::vector<std::unordered_set<VertexId>> depthMap_;
};

}  // namespace uni_course_cpp

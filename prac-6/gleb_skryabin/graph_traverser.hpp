#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {

class GraphTraverser {
 public:
  GraphTraverser(const Graph& graph) : graph_(graph) {}

  GraphPath findShortestPath(const VertexId& vertexSourceId,
                             const VertexId& vertexTargetId) const;

  GraphPath findFastestPath(const VertexId& vertexSourceId,
                            const VertexId& vertexTargetId) const;

  std::vector<GraphPath> findAllPaths() const;

 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp

#pragma once

#include <vector>
#include "graph.hpp"

namespace uni_course_cpp {

class GraphPath {
 public:
  using Distance = int;

  GraphPath(Graph::Edge::Duration duration,
            std::vector<VertexId>&& vertexIds,
            std::vector<EdgeId>&& edgeIds)
      : duration_(duration), vertexIds_(vertexIds), edgeIds_(edgeIds) {}

  Distance getDistance() const;
  const Graph::Edge::Duration& getDuration() const;
  const std::vector<VertexId>& getVertexIds() const;
  const std::vector<EdgeId>& getEdgeIds() const;

 private:
  const Graph::Edge::Duration duration_;
  const std::vector<VertexId> vertexIds_;
  const std::vector<EdgeId> edgeIds_;
};

}  // namespace uni_course_cpp

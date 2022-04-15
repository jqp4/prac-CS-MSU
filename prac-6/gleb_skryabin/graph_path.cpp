#include "graph_path.hpp"

namespace uni_course_cpp {

GraphPath::Distance GraphPath::getDistance() const {
  return edgeIds_.size();
}

const Graph::Edge::Duration& GraphPath::getDuration() const {
  return duration_;
}

const std::vector<VertexId>& GraphPath::getVertexIds() const {
  return vertexIds_;
}

const std::vector<EdgeId>& GraphPath::getEdgeIds() const {
  return edgeIds_;
}

}  // namespace uni_course_cpp

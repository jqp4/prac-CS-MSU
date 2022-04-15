#pragma once

#include <unordered_set>
#include "graph.hpp"
#include "string"

namespace uni_course_cpp {
namespace printing {
namespace json {

std::string edgeToJSON(const Graph::Edge& edge);

std::string vertexToJSON(const VertexId& vertexId,
                         const std::unordered_set<EdgeId>& edgeIds,
                         const Graph::Depth& depth);

std::string graphToJSON(const Graph& graph);

}  // namespace json
}  // namespace printing
}  // namespace uni_course_cpp

#include "graph_json_printing.hpp"
#include <unordered_set>
#include "graph.hpp"
#include "graph_printing.hpp"
#include "string"

namespace uni_course_cpp {
namespace printing {
namespace json {

std::string edgeToJSON(const Graph::Edge& edge) {
  std::string json = "{\"id\": ";
  const auto& vertexIds = edge.vertexIds;
  json += std::to_string(edge.id) + ", \"vertex_ids\": [";
  json += std::to_string(vertexIds.first) + ", ";
  json += std::to_string(vertexIds.second) + "],";
  json += " \"color\": \"" + edgeColorToStr(edge.color);
  json += "\"}";
  return json;
}

std::string vertexToJSON(const VertexId& vertexId,
                         const std::unordered_set<EdgeId>& edgeIds,
                         const Graph::Depth& depth) {
  std::string json = "{\"id\": ";
  json += std::to_string(vertexId);
  json += ", \"edge_ids\": [";

  for (const auto& edgeId : edgeIds) {
    json += std::to_string(edgeId) + ", ";
  }

  json.pop_back();
  json.pop_back();
  json += "], \"depth\": ";
  json += std::to_string(depth) + "}";
  return json;
}

std::string graphToJSON(const Graph& graph) {
  const auto& edges = graph.getEdges();
  const auto& vertices = graph.getVertices();
  std::string json = "{\n\"vertices\": [\n";

  for (const auto& [vertexId, vertex] : vertices) {
    json += vertexToJSON(vertexId, graph.getConnectedEdgeIds(vertexId),
                         graph.getVertexDepth(vertexId));
    json += ",\n";
  }

  json.pop_back();
  json.pop_back();
  json += "\n],\n\"edges\": [\n";

  for (const auto& [edgeId, edge] : edges) {
    json += edgeToJSON(edge) + ",\n";
  }

  json.pop_back();
  json.pop_back();
  json += "\n]\n}\n";
  return json;
}

}  // namespace json
}  // namespace printing
}  // namespace uni_course_cpp

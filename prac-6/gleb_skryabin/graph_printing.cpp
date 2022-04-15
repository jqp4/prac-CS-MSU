#include "graph_printing.hpp"
#include <array>
#include <stdexcept>
#include <unordered_set>
#include "graph.hpp"
#include "string"

namespace {

using std::to_string;
using Color = uni_course_cpp::Graph::Edge::Color;
const std::array<Color, 4> edgeColors = {Color::Gray, ::Color::Green,
                                         Color::Yellow, Color::Red};
}  // namespace

namespace uni_course_cpp {
namespace printing {

std::string edgeColorToStr(const Color& color) {
  switch (color) {
    case Color::Gray:
      return "gray";
    case Color::Green:
      return "green";
    case Color::Yellow:
      return "yellow";
    case Color::Red:
      return "red";
    default:
      throw std::runtime_error("Invalid Color");
  }
}

std::string printGraph(const Graph& graph) {
  std::string output = "{\n  depth: ";
  output += to_string(graph.getDepth()) + ",\n  vertices: {amount: ";
  output += to_string(graph.getVertices().size()) + ", distribution [";

  for (Graph::Depth depth = 0; depth <= graph.getDepth(); depth++) {
    output += to_string(graph.getVertexIdsByDepth(depth).size()) + ", ";
  }

  output.pop_back();
  output.pop_back();
  output += "]},\n  edges: {amount: ";
  output += to_string(graph.getEdges().size()) + ", distribution {";

  for (const Graph::Edge::Color& color : edgeColors) {
    output += edgeColorToStr(color) + ": ";
    output += to_string(graph.getEdgeIdsByColor(color).size()) + ", ";
  }

  output.pop_back();
  output.pop_back();
  output += "}}\n}\n";
  return output;
}

std::string printPath(const GraphPath& path) {
  std::string output = "{vertices: [";
  for (const VertexId& vertexId : path.getVertexIds()) {
    output += to_string(vertexId) + ", ";
  }

  output.pop_back();
  output.pop_back();
  output += "], distance: " + to_string(path.getDistance()) + "}";
  return output;
}

std::string printGame(const uni_course_cpp::Game& game) {
  std::string output = "{\n  map:\n" + printGraph(game.getMap());

  output += ",\n  knight position: {vertex_id: ";
  output += to_string(game.getKnightPosition()) + ", depth: ";
  output += to_string(game.getMap().getVertexDepth(game.getKnightPosition()));

  output += "},\n  princess position: {vertex_id: ";
  output += to_string(game.getPrincessPosition()) + ", depth: ";
  output += to_string(game.getMap().getVertexDepth(game.getPrincessPosition()));

  return output + "}\n}\n";
}

}  // namespace printing
}  // namespace uni_course_cpp

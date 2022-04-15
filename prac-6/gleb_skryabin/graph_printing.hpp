#pragma once

#include "game.hpp"
#include "graph.hpp"
#include "graph_path.hpp"
#include "string"

namespace uni_course_cpp {
namespace printing {

std::string edgeColorToStr(const Graph::Edge::Color& color);
std::string printGraph(const Graph& graph);
std::string printPath(const GraphPath& path);
std::string printGame(const Game& game);

}  // namespace printing
}  // namespace uni_course_cpp

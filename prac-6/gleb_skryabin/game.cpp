#include "game.hpp"
#include "graph_traverser.hpp"

namespace uni_course_cpp {

Graph Game::getMap() const {
  return map_;
}
VertexId Game::getKnightPosition() const {
  return knightPosition_;
}
VertexId Game::getPrincessPosition() const {
  return princessPosition_;
}

GraphPath Game::findShortestPath() const {
  GraphTraverser graphTraverser(map_);
  return graphTraverser.findShortestPath(knightPosition_, princessPosition_);
}

GraphPath Game::findFastestPath() const {
  GraphTraverser graphTraverser(map_);
  return graphTraverser.findFastestPath(knightPosition_, princessPosition_);
}

}  // namespace uni_course_cpp

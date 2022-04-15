#pragma once

#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {

class Game {
 public:
  Game(Graph&& map, VertexId knightPosition, VertexId princessPosition)
      : map_(map),
        knightPosition_(knightPosition),
        princessPosition_(princessPosition) {}

  Graph getMap() const;
  VertexId getKnightPosition() const;
  VertexId getPrincessPosition() const;
  GraphPath findShortestPath() const;
  GraphPath findFastestPath() const;

 private:
  const Graph map_;
  const VertexId knightPosition_;
  const VertexId princessPosition_;
};

}  // namespace uni_course_cpp

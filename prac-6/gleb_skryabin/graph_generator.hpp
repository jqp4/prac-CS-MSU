#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth = 0, int newVerticesCount = 0)
        : depth_(depth), newVerticesCount_(newVerticesCount) {}

    const Graph::Depth getDepth() const { return depth_; }
    const int getNewVerticesCount() const { return newVerticesCount_; }

   private:
    const Graph::Depth depth_ = 0;
    const int newVerticesCount_ = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  float getColorProbability(const Graph::Edge::Color& color,
                            const Graph::Depth& depth) const;

  void generateGrayEdges(Graph& graph, VertexId vertexSourceId) const;

  /// Рекурсивная функция генерации ветки графа, только серые ребра (в начале
  /// граф генерируется как дерево глубиной не больше params_.depth())
  //   void generateGrayBranches(Graph& graph,
  //                             const VertexId& vertexSourceId,
  //                             const Graph::Depth& remainingDepth,
  //                             std::mutex& mutex) const;

  void generateGrayBranch(Graph& graph,
                          Graph::Depth depth,
                          const VertexId& vertex_id,
                          std::mutex& mutex) const;

  void generateGreenEdges(Graph& graph, std::mutex& mutex) const;
  void generateYellowEdges(Graph& graph, std::mutex& mutex) const;
  void generateRedEdges(Graph& graph, std::mutex& mutex) const;

  const Params params_ = Params();
};
}  // namespace uni_course_cpp

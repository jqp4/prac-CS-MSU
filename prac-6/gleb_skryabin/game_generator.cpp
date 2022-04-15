#include "game_generator.hpp"
#include <random>
#include "game.hpp"

namespace {

uni_course_cpp::VertexId getRandomVertexId(
    const std::unordered_set<uni_course_cpp::VertexId>& vertexIds) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_number(0, vertexIds.size() - 1);
  auto it = std::begin(vertexIds);
  std::advance(it, random_number(gen));
  return *it;
}

}  // namespace

namespace uni_course_cpp {

Game GameGenerator::generate() const {
  const auto graphGenerator = GraphGenerator(params_);
  auto map = graphGenerator.generate();
  const auto& lastDepthLyaer = map.getVertexIdsByDepth(map.getDepth());
  return Game(std::move(map), 0, getRandomVertexId(lastDepthLyaer));
}

}  // namespace uni_course_cpp

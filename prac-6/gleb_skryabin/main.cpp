#include <filesystem>
#include <fstream>
#include <iostream>
#include "config.hpp"
#include "game_generator.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
#include "graph_traverser_controller.hpp"
#include "logger.hpp"

namespace {

void writeJSONfile(const std::string& jsonText, const std::string& filename) {
  std::ofstream jsonFile(uni_course_cpp::config::kTempDirectoryPath + filename);
  if (jsonFile.is_open()) {
    jsonFile << jsonText;
    jsonFile.close();
  } else {
    throw std::runtime_error("Unable to open file");
  }
}

void writeGraphJSONfile(const uni_course_cpp::Graph& graph, int index) {
  const std::string filename = "graph_" + std::to_string(index) + ".json";
  std::ofstream json(uni_course_cpp::config::kTempDirectoryPath + filename);
  if (json.is_open()) {
    json << uni_course_cpp::printing::json::graphToJSON(graph);
    json.close();
    std::cout << "Graph " << index << " is written to " << filename << "\n\n";
  } else {
    throw std::runtime_error("Unable to open file");
  }
}

int inputPositiveVariable(const std::string& message) {
  int var;
  std::cout << "Enter " << message << ":\n";
  std::cin >> var;
  while (var < 0) {
    std::cout << "Invalid input " << message;
    std::cout << " <= 0, try again\n";
    std::cin >> var;
  }
  return var;
}

std::string generationStartedLogString(int graphIndex) {
  std::string output = " Graph ";
  output += std::to_string(graphIndex) + ", Generation Started\n";
  return output;
}

std::string generationFinishedLogString(int graphIndex,
                                        const uni_course_cpp::Graph& graph) {
  std::string output = " Graph ";
  output += std::to_string(graphIndex) + ", Generation Finished";
  output += uni_course_cpp::printing::printGraph(graph);
  return output;
}

void prepareTempDirectory() {
  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);
}

std::vector<uni_course_cpp::Graph> generateGraphs(
    const uni_course_cpp::GraphGenerator::Params& params,
    int graphsCount,
    int threadsCount) {
  auto generationController = uni_course_cpp::GraphGenerationController(
      threadsCount, graphsCount, params);

  auto& logger = uni_course_cpp::Logger::getLogger();
  auto graphs = std::vector<uni_course_cpp::Graph>();
  graphs.reserve(graphsCount);

  generationController.generate(
      [&logger](int index) { logger.log(generationStartedLogString(index)); },
      [&logger, &graphs](int index, uni_course_cpp::Graph graph) {
        logger.log(generationFinishedLogString(index, graph));
        writeGraphJSONfile(graph, index);
        graphs.push_back(std::move(graph));
      });

  return graphs;
}

std::string traversalStartedString(int graphIndex) {
  return " Graph " + std::to_string(graphIndex) + ", Traversal Started\n";
}

std::string traversalFinishedString(
    int graphIndex,
    const std::vector<uni_course_cpp::GraphPath>& pathes) {
  std::string output = " Graph " + std::to_string(graphIndex);
  output += ", Traversal Finished, Paths: [\n";
  for (int index = 0; index < pathes.size(); index++) {
    output += "  " + uni_course_cpp::printing::printPath(pathes[index]);
    if (index < pathes.size()) {
      output += ",";
    }
    output += "\n";
  }

  return output + "]\n\n";
}

void traverseGraphs(const std::vector<uni_course_cpp::Graph>& graphs) {
  auto traversalController = uni_course_cpp::GraphTraversalController(graphs);
  auto& logger = uni_course_cpp::Logger::getLogger();

  traversalController.traverse(
      [&logger](int index) { logger.log(traversalStartedString(index)); },
      [&logger](int index, std::vector<uni_course_cpp::GraphPath> paths) {
        logger.log(traversalFinishedString(index, paths));
      });
}

int mainUnder08() {
  const int depth = inputPositiveVariable("depth of all graphs");
  const int newVerticesNum = inputPositiveVariable("number of new vertices");
  const int graphsCount = inputPositiveVariable("number of graphs");
  const int threadsCount = inputPositiveVariable("number of threads");
  prepareTempDirectory();

  const uni_course_cpp::GraphGenerator::Params params(depth, newVerticesNum);
  const auto graphs = generateGraphs(params, graphsCount, threadsCount);

  traverseGraphs(graphs);
  return 0;
}

std::string gamePreparingString() {
  return "Game is Preparing...";
}

std::string gameReadyString(const uni_course_cpp::Game& game) {
  std::string output = "Game is Ready ";
  output += uni_course_cpp::printing::printGame(game);
  return output;
}

std::string shortestPathSearchingString() {
  return "Searching for Shortest Path...";
}
std::string shortestPathReadyString(const uni_course_cpp::GraphPath& path) {
  return "Shortest Path: " + uni_course_cpp::printing::printPath(path);
}

std::string fastestPathSearchingString() {
  return "Searching for Fastest Path...";
}
std::string fastestPathReadyString(const uni_course_cpp::GraphPath& path) {
  return "Fastest Path: " + uni_course_cpp::printing::printPath(path);
}

}  // namespace

int main() {
  const int depth = inputPositiveVariable("depth of all graphs");
  const int newVerticesNum = inputPositiveVariable("number of new vertices");
  prepareTempDirectory();

  auto& logger = uni_course_cpp::Logger::getLogger();
  logger.log(gamePreparingString());

  auto params = uni_course_cpp::GraphGenerator::Params(depth, newVerticesNum);
  const auto gameGenerator = uni_course_cpp::GameGenerator(std::move(params));
  const auto game = gameGenerator.generate();

  logger.log(gameReadyString(game));
  logger.log(shortestPathSearchingString());

  const auto shortestPath = game.findShortestPath();

  logger.log(shortestPathReadyString(shortestPath));
  logger.log(fastestPathSearchingString());

  const auto fastest_path = game.findFastestPath();

  logger.log(fastestPathReadyString(fastest_path));

  const auto map_json =
      uni_course_cpp::printing::json::graphToJSON(game.getMap());
  writeJSONfile(map_json, "map.json");

  return 0;
}
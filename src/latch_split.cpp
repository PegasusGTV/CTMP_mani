#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<std::vector<int>> load_map(const std::string& filename, int rows, int cols) {
    std::ifstream infile(filename);
    std::vector<std::vector<int>> map(rows, std::vector<int>(cols));

    if (!infile.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return map;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            infile >> map[i][j];
        }
    }

    infile.close();
    return map;
}

json meta;
std::ifstream file("map_meta.json");
file >> meta;

std::pair<int, int> start = {meta["start"][0], meta["start"][1]};
std::pair<int, int> goal = {meta["goal"][0], meta["goal"][1]};
std::vector<int> intermediate = meta["intermediate_goal_zone"];
std::vector<int> constraint = meta["constraint_zone"];
int tunnel_width = meta["tunnel_width"];
int d = meta["d"];
int num_tunnels = meta["num_tunnels"];
std::vector<std::vector<int>> tunnel_shape = meta["tunnel_shape"];


int main() {
    int rows = 999, cols = 999;
    std::string filename = "map.txt";
    auto world = load_map(filename, rows, cols);

    // Example: print center cell
    std::cout << "Center value: " << world[rows/2][cols/2] << std::endl;

    return 0;
}
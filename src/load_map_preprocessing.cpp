#include "env_map.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

Map loadMapFromFiles_preprocessing(const std::string& txtPath, const std::string& jsonPath){
    Map map;
    
    std::ifstream jsonFile(jsonPath);
    if(!jsonFile.is_open()) throw std::runtime_error("Could not open JSON file.");
    
    json meta;
    jsonFile >> meta;
    map.start = {meta["start"][0], meta["start"][1]};
    map.goal = {meta["goal"][0], meta["goal"][1]};
    map.intermediate_goal_zone = meta["intermediate_goal_zone"].get<std::vector<int>>();
    map.constraint_zone = meta["constraint_zone"].get<std::vector<int>>();
    map.tunnel_width = meta["tunnel_width"];
    map.d = meta["d"];
    map.num_tunnels = meta["num_tunnels"];
    map.tunnel_path = meta["tunnel_path"];

    std::ifstream txtFile(txtPath);
    if (!txtFile.is_open()) throw std::runtime_error("Could not open map file.");
    std::string line;
    std::vector<std::vector<int>> grid;
    while (std::getline(txtFile, line)) {
        std::istringstream ss(line);
        std::vector<int> row;
        int value;
        while(ss >> value){
            row.push_back(value);
        }
        if (!row.empty()) grid.push_back(row);
    }

    map.occupancy_grid = grid;
    map.y_length = grid.size();
    map.x_length = grid.empty() ? 0 : grid[0].size();
    return map;
}

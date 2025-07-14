#include "point.h"
#include "env_map.h"
#include "reach_constraint.h"
#include "tunnel_preprocessing.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

int main(){
    const std::string map_txt  = "../maps/map.txt";
    const std::string map_meta = "../maps/map_meta.json";
    const std::string out_json  = "../maps/online_data.json";
    const std::string out_bin  = "../data/test_tunnels.dat";
    const std::string rc_file  = "../data/reach_constraint.dat";

    Map env = loadMapFromFiles_preprocessing(map_txt, map_meta);

    std::ifstream jsonFile(out_json);
    if(!jsonFile.is_open()) throw std::runtime_error("Could not open JSON file.");

    json test_map;
    jsonFile >> test_map;
    // env.occupancy_grid = test_map["map"]["occupancy_grid"].get<std::vector<std::vector<int>>>();
    // std::cout << "Map loaded: "
    //           << env.x_length << "×" << env.y_length
    //           << "  start=(" << env.start.first  << "," << env.start.second << ")"
    //           << "  goal =(" << env.goal.first   << "," << env.goal.second  << ")\n";
    const auto& occ = test_map["occupancy"];
    env.occupancy_grid.resize(env.y_length, std::vector<int>(env.x_length));
    for (int y = 0; y < env.y_length; ++y) {
        for (int x = 0; x < env.x_length; ++x) {
            env.occupancy_grid[y][x] = occ[y][x];
        }
    }
    // std::cout << "Occupancy grid loaded with size: "
    //           << env.occupancy_grid.size() << " rows, "
    //           << env.occupancy_grid[0].size() << " columns.\n";

    Point intermediate_goal_point(
        test_map["intermediate_goal"]["x"],
        test_map["intermediate_goal"]["y"]
    );

    TunnelPreprocessor pre(env);
    if (!pre.loadTunnelsFromFile(out_bin)) {
      throw std::runtime_error("Failed to load tunnels from " + out_bin);
    }

    std::cout << "Reloaded " << pre.getTunnels().size() << " tunnels successfully.\n";

    std::cout<< "number of tunnel groups: " << pre.num_tunnels_in_group_ << "\n";
    std::cout<< "number of tunnels: " << pre.num_tunnels_ << "\n";
    std::cout<< "number of tunnel groups: " << pre.num_tunnel_groups_ << "\n";


    ReachConstraint rc(env);
    if (!rc.loadFromFile(rc_file)) {
      throw std::runtime_error("Failed to load ReachConstraint from " + rc_file);
    }

    std::cout << "number of tunnel groups: " << rc.tunnel_groups_.size() << "\n";
    std::cout << "number of root paths to tunnel groups: " << rc.root_paths_to_tunnel_groups_.size() << "\n";






    return 0;
}
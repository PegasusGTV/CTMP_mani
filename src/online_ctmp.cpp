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
    const std::string final_path_file = "../data/final_path.json";

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

    int x_for_tunnel_checking = env.constraint_zone[0];
    int tunnel_number = 0;
    for(int i = 0; i < env.y_length - env.tunnel_width + 1; i++){
        if(env.occupancy_grid[i][x_for_tunnel_checking] == 0){
            tunnel_number = i;
            break;
        }
    }

    int tunnel_group = static_cast<int>(tunnel_number / env.tunnel_width);

    std::vector<Point> part_5 = pre.paths_to_solve_tunnel_constraints_[tunnel_group].path;

    std::vector<Point> part_6 = rc.root_paths_to_goal_[tunnel_group][0].root_path;

    std::vector<Point> part_1 = rc.root_paths_to_tunnel_groups_[tunnel_group][0].pivot_path;
    Point to_intermediate_goal_pivot_point = rc.root_paths_to_tunnel_groups_[tunnel_group][0].pivot_point;
    std::vector<Point> part_2 = WaStar(env.occupancy_grid, to_intermediate_goal_pivot_point, intermediate_goal_point, 2.0);

    std::cout<< "to_intermediate_goal_pivot_point: " << to_intermediate_goal_pivot_point.x << ", " << to_intermediate_goal_pivot_point.y << "\n";
    std::cout<< "intermediate_goal_point: " << intermediate_goal_point.x << ", " << intermediate_goal_point.y << "\n";


    std::vector<Point> part_4 = rc.root_paths_from_tunnel_groups_[tunnel_group][0].pivot_path;
    Point from_intermediate_goal_pivot_point = rc.root_paths_from_tunnel_groups_[tunnel_group][0].pivot_point;
    std::vector<Point> part_3 = WaStar(env.occupancy_grid, intermediate_goal_point, from_intermediate_goal_pivot_point, 1.0);

    std::cout<< "from_intermediate_goal_pivot_point: " << from_intermediate_goal_pivot_point.x << ", " << from_intermediate_goal_pivot_point.y << "\n";

    std::vector<Point> final_path;
    final_path.insert(final_path.end(), part_1.begin(), part_1.end());
    // final_path.insert(final_path.end(), part_2.begin(), part_2.end());
    // final_path.insert(final_path.end(), part_3.begin(), part_3.end());
    // final_path.insert(final_path.end(), part_4.begin(), part_4.end());
    // final_path.insert(final_path.end(), part_5.begin(), part_5.end());
    // final_path.insert(final_path.end(), part_6.begin(), part_6.end());

    std::cout << "Part 1 size: " << part_1.size() << "\n";
    std::cout << "Part 2 size: " << part_2.size() << "\n";
    std::cout << "Part 3 size: " << part_3.size() << "\n";

    std::ofstream path_out(final_path_file);
    if (!path_out) throw std::runtime_error("Could not open final_path.json for writing");

    json path_json = json::array();
    for (const auto& p : final_path) {
        path_json.push_back({{"x", p.x}, {"y", p.y}});
    }

    path_out << path_json.dump(2);  // pretty-print with 2-space indent
    path_out.close();

    std::cout << "Saved final path to ../data/final_path.json\n";






    return 0;
}
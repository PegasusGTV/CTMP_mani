#include "env_map.h"
#include "point.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <random>
#include <fstream>
using json = nlohmann::json;

int main(){
    const std::string map_txt  = "../maps/map.txt";
    const std::string map_meta = "../maps/map_meta.json";

    std::mt19937 rng(std::random_device{}());

    Map Online_map = loadMapFromFiles_preprocessing(map_txt, map_meta);

    // int x_length = Online_map.x_length;

    int number_of_tunnels = Online_map.x_length - Online_map.tunnel_width + 1;
    // std::cout << "Number of tunnels: " << number_of_tunnels << "\n";
    int num_of_groups = number_of_tunnels/Online_map.tunnel_width;

    std::uniform_int_distribution<int> dist(0, number_of_tunnels - 1);

    int choice = dist(rng);

    // int tunnel_start, tunnel_end = choice, choice + Online_map.tunnel_width - 1; wrong
    int tunnel_start = choice;
    int tunnel_end   = choice + Online_map.tunnel_width - 1;

    // int tunnel_start = choice,
    // tunnel_end   = choice + Online_map.tunnel_width - 1; correct

    int constraint_start = Online_map.constraint_zone[0];
    int constraint_end = Online_map.constraint_zone[1];

    int intermediate_goal_start = Online_map.intermediate_goal_zone[0];
    int intermediate_goal_end = Online_map.intermediate_goal_zone[1];

    for (int x = constraint_start; x <= constraint_end; ++x) {
        for (int y = tunnel_start; y <= tunnel_end; ++y) {
            Online_map.occupancy_grid[y][x] = 0; // Mark as free
        }
    }

    std::uniform_int_distribution<int> dist_y(tunnel_start, tunnel_end);
    std::uniform_int_distribution<int> dist_x(intermediate_goal_start, intermediate_goal_end);

    int intermediate_goal_x = dist_x(rng);
    int intermediate_goal_y = dist_y(rng);

    Point intermediate_goal_point(intermediate_goal_x, intermediate_goal_y);


    json j;
    j["map"] = {
    {"width",  Online_map.x_length},
    {"height", Online_map.y_length},
    {"start",  {{"x", Online_map.start.first}, {"y", Online_map.start.second}}},
    {"goal",   {{"x", Online_map.goal.first},  {"y", Online_map.goal.second}}}
    };
    // dump the *modified* grid
    j["occupancy"] = json::array();
    for(int y = 0; y < Online_map.y_length; ++y) {
    j["occupancy"].push_back(json::array());
    for(int x = 0; x < Online_map.x_length; ++x){
        j["occupancy"][y].push_back(Online_map.occupancy_grid[y][x]);
    }
    }

    // 2) Dump your tunnels as before
    // j["tunnels"] = json::array();
    // for (auto const &t : tunnels) {
    // json tj;
    // tj["id"]     = t.id;
    // tj["points"] = json::array();
    // for (auto const &p : t.points) {
    //     tj["points"].push_back({{"x", p.x}, {"y", p.y}});
    // }
    // j["tunnels"].push_back(std::move(tj));
    // }

    // 3) And *now* serialize your new intermediate goal
    j["intermediate_goal"] = {
    {"x", intermediate_goal_x},
    {"y", intermediate_goal_y}
    };

    // 4) Finally write it all out
    std::ofstream ofs("./maps/online_data.json");
    if(!ofs) throw std::runtime_error("couldn’t open online_data.json");
    ofs << j.dump(2) << "\n";
    std::cout << "Wrote map + goal + tunnels → online_data.json\n";


}
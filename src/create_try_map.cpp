#include "env_map.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <random>
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
    




}
#include "env_map.h"
#include<iostream>
#include<fstream>

int main() {
    try {
        Map env = loadMapFromFiles_preprocessing("../maps/map.txt", "../maps/map_meta.json");

        std::cout << "Map size: " << env.x_length << " x " << env.y_length << "\n";
        std::cout << "Start: (" << env.start.first << ", " << env.start.second << ")\n";
        std::cout << "Goal: (" << env.goal.first << ", " << env.goal.second << ")\n";
        std::cout << "Intermediate Zone: [" << env.intermediate_goal_zone[0] << ", " 
                  << env.intermediate_goal_zone[1] << "]\n";

        // You can now use env.grid, env.tunnel_shape, etc. in planning

    } catch (const std::exception& e) {
        std::cerr << "Error loading map: " << e.what() << std::endl;
    }
    return 0;
}
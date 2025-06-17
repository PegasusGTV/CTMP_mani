#ifndef ENV_MAP_H
#define ENV_MAP_H

#include <vector>
#include <string>

struct Map{
    int x_length;
    int y_length;
    std::vector<std::vector<int>> occupancy_grid; // 0 for free, 1 for occupied
    std::pair<int, int> start; // (x, y) coordinates of the start position
    std::pair<int, int> goal; // (x, y) coordinates of the goal position
    std::vector<int> intermediate_goal_zone;
    std::vector<int> constraint_zone;
    int tunnel_width;
    int d;
    int num_tunnels;
    std::vector<std::pair<int, int>> tunnel_path;
};


Map loadMapFromFiles_preprocessing(const std::string& txtPath, const std::string& jsonPath);

#endif // ENV_MAP_H
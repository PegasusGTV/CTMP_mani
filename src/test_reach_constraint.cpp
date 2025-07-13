// #include "reach_constraint.h"
// #include <iostream>
// #include <fstream>

// int main() {
//     // Step 1: Setup a dummy map
//     Map map;
//     map.x_length = 10;
//     map.y_length = 10;
//     map.start = {0, 5};
//     map.goal = {9, 5};
//     map.constraint_zone = {3, 7};
//     map.intermediate_goal_zone = {2, 6};
//     map.tunnel_width = 2;
//     map.occupancy_grid = std::vector<std::vector<int>>(10, std::vector<int>(10, 0)); // All free

//     // Step 2: Create dummy tunnels
//     Tunnel tunnel;
//     tunnel.id = 1;
//     tunnel.start = {5, 5};
//     tunnel.points = {{5,5}, {5,6}, {5,7}};

//     std::vector<Tunnel> tunnels = {tunnel};

//     // Step 3: Create dummy tunnel group
//     TunnelGroup group;
//     group.id = 10;
//     group.representative = tunnel;
//     group.Tunnels = {tunnel};

//     std::map<int, TunnelGroup> tunnel_groups = {
//         {10, group}
//     };

//     // Step 4: Construct ReachConstraint and compute
//     ReachConstraint reach(map);
//     reach.findIntermediateGoalRegions(tunnels, tunnel_groups, 0.01, 0.01);
//     reach.findRootPathsToTunnelGroups();
    
//     std::string filename = "reach_test_save.dat";

//     if (reach.saveToFile(filename)) {
//         std::cout << "Saved ReachConstraint successfully.\n";
//     } else {
//         std::cerr << "Failed to save ReachConstraint.\n";
//     }

//     ReachConstraint loaded_reach(map);
//     if (loaded_reach.loadFromFile(filename)) {
//         std::cout << "Loaded ReachConstraint successfully.\n";
//     } else {
//         std::cerr << "Failed to load ReachConstraint.\n";
//     }

//     return 0;
// }

#include "reach_constraint.h"
// class ReachConstraint {
// public:
ReachConstraint::ReachConstraint(const Map& map) : map_(map), grid_no_constraint(map) {}

void ReachConstraint::findIntermediateGoalRegions(std::vector<Tunnel> tunnels, 
                                    std::map<int, TunnelGroup> tunnel_groups,
                                double t_bound_1, double t_bound_2, double t_bound_3) {
    // Implementation to find intermediate goal regions per tunnel
    std::cout<< map_.constraint_zone[0] << ", " << map_.constraint_zone[1] << std::endl;
    for (int x = map_.constraint_zone[0]; x <= map_.constraint_zone[1]; ++x) {
        for (int y = 0; y < map_.occupancy_grid[0].size(); ++y) {
            grid_no_constraint.occupancy_grid[y][x] = 0; // Mark as free
        }
    }

    // for(int x = 0; x<grid_no_constraint.occupancy_grid.size(); ++x) {
    //     for(int y = 0; y < grid_no_constraint.occupancy_grid[0].size(); ++y) {
    //         std::cout<< grid_no_constraint.occupancy_grid[x][y] << ",";
    //     }
    //     std::cout<< std::endl;
    // }
    // int count = 0;
    // for(int x = 0; x<grid_no_constraint.occupancy_grid.size(); ++x) {
    //     for(int y = 0; y < grid_no_constraint.occupancy_grid[0].size(); ++y) {
    //         count += grid_no_constraint.occupancy_grid[x][y];

    //     }
    //     std::cout<< "count: " << count << std::endl;
    //     // std::cout<< std::endl;
    // }
    std::vector<int> intermediate_goal_zone = map_.intermediate_goal_zone;
    int tunnel_width = map_.tunnel_width;
    tunnels_ = tunnels;
    tunnel_groups_ = tunnel_groups;
    for (const auto& tunnel : tunnels_) {
        IntermediateGoalRegionperTunnel region;
        region.id = tunnel.id;
        region.tunnel = tunnel;
        for (int i = intermediate_goal_zone[0]; i <= intermediate_goal_zone[1]; ++i) {
            for(int j = tunnel.start.y - static_cast<int>(tunnel_width/2); j <= tunnel.start.y + static_cast<int>(tunnel_width/2); ++j) {
                region.intermediate_goal_region.push_back(Point(i, j));
                // std::cout << "intermediate_goal_region: " << i << "," << j << "   ." ;
            }
        }
        // std::cout << std::endl;

        intermediate_goal_regions_per_tunnel_[region.id] = region;
    }

    for(const auto& group : tunnel_groups_) {
        IntermediateGoalRegionperTunnelGroup region_group;
        region_group.id = group.first;
        region_group.tunnel_group_id = group.first;

        // std::cout << "representative point: " << group.second.representative.start.x << "," << group.second.representative.start.y << std::endl;
        for (int i = intermediate_goal_zone[0]; i <= intermediate_goal_zone[1]; ++i) {
            for(int j = group.second.representative.start.y - tunnel_width + 1; j <= group.second.representative.start.y + tunnel_width - 1; ++j) {
                region_group.intermediate_goal_region.push_back(Point(i, j));
                // std::cout << i << "," << j << "   ." ;
            }
        }
        // std::cout << std::endl;
        intermediate_goal_regions_per_tunnel_group_[region_group.id] = region_group;
    }

    t_bound_1_ = t_bound_1;
    t_bound_2_ = t_bound_2;
    t_bound_3_ = t_bound_3;

    std::cout << "Intermediate goal regions per tunnel: " << intermediate_goal_regions_per_tunnel_.size() << std::endl;
    std::cout << "Intermediate goal regions per tunnel group: " << intermediate_goal_regions_per_tunnel_group_.size() << std::endl;
    std::cout << "Intermediate goal points regions per tunnel group: " << intermediate_goal_regions_per_tunnel_group_[99].intermediate_goal_region.size() << std::endl;
    std::cout << "Tunnels: " << tunnels_.size() << std::endl;
}

void ReachConstraint::findRootPathsToTunnelGroups() {
    // Implementation to find root paths to tunnel groups
    // std::cout << "enter findRootPathsToTunnelGroups()\n" << std::flush;
    // std::cout<< intermediate_goal_regions_per_tunnel_group_.size() << std::endl;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(const auto& group : intermediate_goal_regions_per_tunnel_group_){
        int root_path_number = 0;
        
        std::vector<Point> region_to_cover = group.second.intermediate_goal_region;
        // if(root_path_number == 0){
        // std::cout<< "passed thriugh this point" << std::endl;
        // std::cout<< "region_to_cover size: " << region_to_cover.size() << std::endl;
        // }

        while(!region_to_cover.empty()) {
            auto it = region_to_cover.begin();
            std::advance(it, rand() % region_to_cover.size());
            Point root_goal = *it;
            Point start(map_.start.first, map_.start.second);
            // if(root_path_number == 0){
            // std::cout<< "root_goal: " << root_goal.x << ", " << root_goal.y << std::endl;
            // std::cout<< "size of region_to_cover: " << region_to_cover.size() << std::endl;
                // std::cout<< "start: " << start.x << ", " << start.y << std::endl;
            // }

            std::vector<Point> path = WaStar(grid_no_constraint.occupancy_grid, start, root_goal, 1.0);
            // std::cout<< "path size: " << path.size() << std::endl;
            // if(!path.empty()) {
            //     std::cout<< "path size: " << path.size() << std::endl;
            // }
            if( !path.empty()) {
                // std::cout << "reached inside \n" << std::flush;
                RootPathtoTunnelGroup root_path;
                root_path.id = root_path_number++;
                root_path.tunnel_group_id = group.first;
                root_path.root_path = path;
                root_path.start = start;
                root_path.end = root_goal;
                root_path.t_bound_1 = t_bound_1_;
                int perception_x = map_.intermediate_goal_zone[0] - perception_radius_;

                auto pivot = std::find_if(path.begin(), path.end(), [&](const Point& p) {
                    return p.x == perception_x;
                });

                // if(pivot != path.end()) {
                Point pivot_point = *pivot;
                // }

                root_path.region_covered_by_root_path.push_back(root_goal);

                // std::cout<< "pivot_point: " << pivot_point.x << ", " << pivot_point.y << std::endl;

                for(auto const& region_point : region_to_cover){
                    auto start_time = std::chrono::high_resolution_clock::now();
                    // std::cout<< "region_point: " << region_point.x << ", " << region_point.y << std::endl;
                    std::vector<Point> path_region = WaStar(grid_no_constraint.occupancy_grid, pivot_point, region_point, 2.0);
                    // std::cout<< "path_region size: " << path_region.size() << std::endl;
                    auto end_time = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end_time - start_time;
                    double planning_time = elapsed.count(); // seconds

                    if(!path_region.empty() && planning_time < t_bound_1_) {
                        root_path.region_covered_by_root_path.push_back(region_point);
                    }
                    // std::cout<< "region_point: " << region_point.x << ", " << region_point.y << std::endl;  
                }

                // root_paths_to_tunnel_groups_[group.first].push_back(root_path);

                std::vector<Point> pivot_path = WaStar(grid_no_constraint.occupancy_grid, start, pivot_point, 1.0);

                root_path.pivot_path = pivot_path;
                root_path.pivot_point = pivot_point;

                root_paths_to_tunnel_groups_[group.first].push_back(root_path);
                // std::cout<< "root_path.region_covered_by_root_path size: " << root_path.region_covered_by_root_path.size() << std::endl;
                region_to_cover.erase(
                std::remove_if(region_to_cover.begin(), region_to_cover.end(),
                    [&](const Point& p) {
                        return std::find(root_path.region_covered_by_root_path.begin(),
                                        root_path.region_covered_by_root_path.end(), p)
                            != root_path.region_covered_by_root_path.end();
                    }),
                region_to_cover.end());
                // std::cout << "exit  findRootPathsToTunnelGroups() 111 \n" << std::endl;
            }
            // std::cout << "exit  findRootPathsToTunnelGroups() 222 \n" << std::endl;
        }
        // std::cout << "exit  findRootPathsToTunnelGroups() 333 \n" << std::endl;
    }
    // std::cout << "exit  findRootPathsToTunnelGroups() 444 \n" << std::endl;
    
}

void ReachConstraint::findRootPathsFromTunnelGroups() {
    // Implementation to find root paths from tunnel groups
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(const auto& group : intermediate_goal_regions_per_tunnel_group_){
        int root_path_number = 0;
        std::vector<Point> region_to_cover = group.second.intermediate_goal_region;
        // std::cout<< "passed thriugh this point 2" << std::endl;
        // std::cout<< "region_to_cover size: 2  " << region_to_cover.size() << std::endl;
        while(!region_to_cover.empty()) {
            auto it = region_to_cover.begin();
            std::advance(it, rand() % region_to_cover.size());
            Point root_start = *it;
            Point constraint_start = tunnel_groups_[group.second.tunnel_group_id].representative.start;

            // std::cout<< "root_start: " << root_start.x << ", " << root_start.y << std::endl;

            std::vector<Point> path = WaStar(grid_no_constraint.occupancy_grid, root_start, constraint_start, 1.0);
            // std::cout<< "path size: " << path.size() << std::endl;
            if(!path.empty()){
                // std::cout << "reached inside 2 \n" << std::flush;
                RootPathFromTunnelGroup root_path;
                root_path.id = root_path_number++;
                root_path.tunnel_group_id = group.first;
                root_path.root_path = path;
                root_path.start = root_start;
                root_path.end = constraint_start;
                root_path.t_bound_2 = t_bound_2_;
                int perception_x = map_.intermediate_goal_zone[1] - perception_radius_;
                // std::cout<< "perception_x: " << perception_x << std::endl;

                auto pivot = std::find_if(path.begin(), path.end(), [&](const Point& p) {
                    return p.x == perception_x;
                });
                Point pivot_point;
                // std::cout<< "pivot size: " << path.size() << std::endl;

                if (pivot != path.end()) {
                    pivot_point = *pivot;
                } else {
                    // fallback: use the first point where p.x > perception_x
                    auto pivot_gt = std::find_if(path.begin(), path.end(), [&](const Point& p) {
                        return p.x > perception_x;
                    });

                    if (pivot_gt != path.end()) {
                        pivot_point = *pivot_gt;
                    } else {
                        // ultimate fallback: just use path.begin() if nothing matches
                        pivot_point = *path.begin();
                    }
                }
                // std::cout<< "pivot_point: " << pivot_point.x << ", " << pivot_point.y << std::endl;
                root_path.region_covered_by_root_path.push_back(root_start);

                for(auto const& region_point : region_to_cover){
                    auto start_time = std::chrono::high_resolution_clock::now();
                    // std::cout<< "region_point: " << region_point.x << ", " << region_point.y << std::endl;
                    std::vector<Point> path_region = WaStar(grid_no_constraint.occupancy_grid,region_point,  pivot_point, 2.0);
                    // std::cout<< "path_region size: " << path_region.size() << std::endl;
                    auto end_time = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end_time - start_time;
                    double planning_time = elapsed.count(); // seconds

                    if(!path_region.empty() && planning_time < t_bound_2_) {
                        root_path.region_covered_by_root_path.push_back(region_point);
                    }  
                }
                // std::cout<< "root_path.region_covered_by_root_path size: " << root_path.region_covered_by_root_path.size() << std::endl;

                std::vector<Point> pivot_path = WaStar(grid_no_constraint.occupancy_grid, pivot_point, constraint_start, 1.0);

                root_path.pivot_path = pivot_path;
                root_path.pivot_point = pivot_point;

                root_paths_from_tunnel_groups_[group.first].push_back(root_path);
                region_to_cover.erase(
                std::remove_if(region_to_cover.begin(), region_to_cover.end(),
                    [&](const Point& p) {
                        return std::find(root_path.region_covered_by_root_path.begin(),
                                        root_path.region_covered_by_root_path.end(), p)
                            != root_path.region_covered_by_root_path.end();
                    }),
                region_to_cover.end());
                // std::cout << "exit  findRootPathsFromTunnelGroups() 111 \n" << std::endl;
            }
            // std::cout << "exit  findRootPathsFromTunnelGroups() 222 \n" << std::endl;
        }
        // std::cout << "exit  findRootPathsFromTunnelGroups() 333 \n" << std::endl;
    }
    // std::cout << "exit  findRootPathsFromTunnelGroups() 444 \n" << std::endl;

}

void ReachConstraint::findRootPathsToGoal() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int root_path_number = 0;
    for(const auto& group : tunnel_groups_){
        RootPathConstrainttoGoal root_path;
        root_path.id = root_path_number++;
        root_path.tunnel_group_id = group.first;
        root_path.start = group.second.representative.end;
        root_path.end = Point(map_.goal.first, map_.goal.second);
        root_path.t_bound_3 = t_bound_3_;
        std::vector<Point> path_to_goal = WaStar(grid_no_constraint.occupancy_grid, 
                                                 root_path.start, 
                                                 root_path.end, 1.0);
        root_path.root_path = path_to_goal;
        root_paths_to_goal_[root_path.tunnel_group_id].push_back(root_path);
    }
}

    // bool saveToFile(const std::string& filename) const {
    //     // Implementation to save reach constraints to file
    //     return true;
    // }

    // bool loadFromFile(const std::string& filename) {
    //     // Implementation to load reach constraints from file
    //     return true;
    // }

bool ReachConstraint::saveToFile(const std::string& filename) const {
    try {
        std::ofstream ofs(filename);
        if (!ofs.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << "\n";
            return false;
        }

        boost::archive::text_oarchive oa(ofs);
        oa << tunnels_;
        oa << tunnel_groups_;
        oa << intermediate_goal_regions_per_tunnel_;
        oa << intermediate_goal_regions_per_tunnel_group_;
        oa << root_paths_to_tunnel_groups_;
        oa << root_paths_from_tunnel_groups_;
        oa << root_paths_to_goal_;
        oa << t_bound_1_;
        oa << t_bound_2_;
        oa << t_bound_3_;

        std::cout << "Saved ReachConstraint to file: " << filename << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during save: " << e.what() << "\n";
        return false;
    }
}

bool ReachConstraint::loadFromFile(const std::string& filename) {
    try {
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            std::cerr << "Failed to open file for reading: " << filename << "\n";
            return false;
        }

        boost::archive::text_iarchive ia(ifs);
        ia >> tunnels_;
        ia >> tunnel_groups_;
        ia >> intermediate_goal_regions_per_tunnel_;
        ia >> intermediate_goal_regions_per_tunnel_group_;
        ia >> root_paths_to_tunnel_groups_;
        ia >> root_paths_from_tunnel_groups_;
        ia >> root_paths_to_goal_;
        ia >> t_bound_1_;
        ia >> t_bound_2_;
        ia >> t_bound_3_;

        std::cout << "Loaded ReachConstraint from file: " << filename << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception during load: " << e.what() << "\n";
        return false;
    }
}

// private:
//     Map map_;
//     std::vector<Tunnel> tunnels_;
//     std::map<int, TunnelGroup> tunnel_groups_;
//     std::map<int, IntermediateGoalRegionperTunnel> intermediate_goal_regions_per_tunnel_;
//     std::map<int, IntermediateGoalRegionperTunnelGroup> intermediate_goal_regions_per_tunnel_group_;
//     double t_bound_1_;
//     double t_bound_2_;
//     std::map<int, std::vector<RootPathtoTunnelGroup>> root_paths_to_tunnel_groups_;
//     std::map<int, std::vector<RootPathFromTunnelGroup>> root_paths_from_tunnel_groups_;
//     // std::vector<IntermediateGoalRegionperTunnel> intermediate_goal_regions_;
//     // std::vector<RootPathtoTunnelGroup> root_paths_to_tunnel_groups_;
//     // std::vector<RootPathFromTunnelGroup> root_paths_from_tunnel_groups_;
//     int perception_radius_ = 10; // Default perception radius


// };
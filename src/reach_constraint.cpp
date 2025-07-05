#include "env_map.h"
#include "tunnel_preprocessing.h"
#include "search/include/wastar.h"
#include<iostream>
#include<fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/map.hpp>
#include <chrono>

struct IntermediateGoalRegionperTunnel{
    int id;
    Tunnel tunnel;
    std::vector<Point> intermediate_goal_region;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & id & tunnel & intermediate_goal_region;
    }
};

struct IntermediateGoalRegionperTunnelGroup{
    int id;
    int tunnel_group_id;
    std::vector<Point> intermediate_goal_region;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & id & tunnel_group_id & intermediate_goal_region;
    }
};

struct RootPathtoTunnelGroup{
    int id;
    int tunnel_group_id;
    std::vector<Point> root_path;
    std::vector<Point> region_covered_by_root_path;
    Point start;
    Point end;
    double t_bound_1;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & id & tunnel_group_id & root_path & region_covered_by_root_path & start & end & t_bound_1;
    }
};

struct RootPathFromTunnelGroup{
    int id;
    int tunnel_group_id;
    std::vector<Point> root_path;
    std::vector<Point> region_covered_by_root_path;
    Point start;
    Point end;
    double t_bound_2;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & id & tunnel_group_id & root_path & region_covered_by_root_path & start & end & t_bound_2;
    }
};

class ReachConstraint {
public:
    ReachConstraint(const Map& map) : map_(map) {}

    void findIntermediateGoalRegions(std::vector<Tunnel> tunnels, 
                                     std::map<int, TunnelGroup> tunnel_groups,
                                    double t_bound_1, double t_bound_2) {
        // Implementation to find intermediate goal regions per tunnel
        std::vector<int> intermdiate_goal_zone = map_.intermediate_goal_zone;
        int tunnel_width = map_.tunnel_width;
        tunnels_ = tunnels;
        tunnel_groups_ = tunnel_groups;
        for (const auto& tunnel : tunnels_) {
            IntermediateGoalRegionperTunnel region;
            region.id = tunnel.id;
            region.tunnel = tunnel;
            for (int i = intermdiate_goal_zone[0]; i <= intermdiate_goal_zone[1]; ++i) {
                for(int j = tunnel.start.y - static_cast<int>(tunnel_width/2) - 1; j <= tunnel.start.y + tunnel_width/2; ++j) {
                    region.intermediate_goal_region.push_back(Point(i, j));
                }
            }
            intermediate_goal_regions_per_tunnel_[region.id] = region;
        }

        for(const auto& group : tunnel_groups_) {
            IntermediateGoalRegionperTunnelGroup region_group;
            region_group.id = group.first;
            region_group.tunnel_group_id = group.first;
            for (int i = intermdiate_goal_zone[0]; i <= intermdiate_goal_zone[1]; ++i) {
                for(int j = group.second.representative.start.y - tunnel_width - 1; j <= group.second.representative.start.y + tunnel_width; ++j) {
                    region_group.intermediate_goal_region.push_back(Point(i, j));
                }
            }
            intermediate_goal_regions_per_tunnel_group_[region_group.id] = region_group;
        }

        t_bound_1_ = t_bound_1;
        t_bound_2_ = t_bound_2;
    }

    void findRootPathsToTunnelGroups() {
        // Implementation to find root paths to tunnel groups
        for(const auto& group : intermediate_goal_regions_per_tunnel_group_){
            int root_path_number = 0;
            std::vector<Point> region_to_cover = group.second.intermediate_goal_region;
            while(!region_to_cover.empty()) {
                auto it = region_to_cover.begin();
                std::advance(it, rand() % region_to_cover.size());
                Point root_goal = *it;
                Point start(map_.start.first, map_.start.second);

                std::vector<Point> path = WaStar(map_.occupancy_grid, start, root_goal, 1.0);
                if( !path.empty()) {
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

                    for(auto const& region_point : region_to_cover){
                        auto start_time = std::chrono::high_resolution_clock::now();
                        std::vector<Point> path_region = WaStar(map_.occupancy_grid, pivot_point, region_point, 2.0);
                        auto end_time = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> elapsed = end_time - start_time;
                        double planning_time = elapsed.count(); // seconds

                        if(!path_region.empty() && planning_time < t_bound_1_) {
                            root_path.region_covered_by_root_path.push_back(region_point);
                        }  
                    }

                    root_paths_to_tunnel_groups_[group.first].push_back(root_path);
                    region_to_cover.erase(
                    std::remove_if(region_to_cover.begin(), region_to_cover.end(),
                        [&](const Point& p) {
                            return std::find(root_path.region_covered_by_root_path.begin(),
                                            root_path.region_covered_by_root_path.end(), p)
                                != root_path.region_covered_by_root_path.end();
                        }),
                    region_to_cover.end());
                }
            }
        }
        
    }

    void findRootPathsFromTunnelGroups() {
        // Implementation to find root paths from tunnel groups
    }

    bool saveToFile(const std::string& filename) const {
        // Implementation to save reach constraints to file
        return true;
    }

    bool loadFromFile(const std::string& filename) {
        // Implementation to load reach constraints from file
        return true;
    }

private:
    Map map_;
    std::vector<Tunnel> tunnels_;
    std::map<int, TunnelGroup> tunnel_groups_;
    std::map<int, IntermediateGoalRegionperTunnel> intermediate_goal_regions_per_tunnel_;
    std::map<int, IntermediateGoalRegionperTunnelGroup> intermediate_goal_regions_per_tunnel_group_;
    double t_bound_1_;
    double t_bound_2_;
    std::map<int, std::vector<RootPathtoTunnelGroup>> root_paths_to_tunnel_groups_;
    std::map<int, RootPathFromTunnelGroup> root_paths_from_tunnel_groups_;
    // std::vector<IntermediateGoalRegionperTunnel> intermediate_goal_regions_;
    std::vector<RootPathtoTunnelGroup> root_paths_to_tunnel_groups_;
    std::vector<RootPathFromTunnelGroup> root_paths_from_tunnel_groups_;
    int perception_radius_ = 10; // Default perception radius


};
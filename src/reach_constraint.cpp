#include "env_map.h"
#include "tunnel_preprocessing.h"
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
    // std::vector<IntermediateGoalRegionperTunnel> intermediate_goal_regions_;
    std::vector<RootPathtoTunnelGroup> root_paths_to_tunnel_groups_;
    std::vector<RootPathFromTunnelGroup> root_paths_from_tunnel_groups_;


};
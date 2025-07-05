#ifndef REACH_CONSTRAINT_H
#define REACH_CONSTRAINT_H
#pragma once
#include "env_map.h"
#include "tunnel_preprocessing.h"
#include "wastar.h"
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
    ReachConstraint(const Map& map);

    void findIntermediateGoalRegions(std::vector<Tunnel> tunnels, 
                                     std::map<int, TunnelGroup> tunnel_groups,
                                    double t_bound_1, double t_bound_2);

    void findRootPathsToTunnelGroups();

    void findRootPathsFromTunnelGroups();
    // bool saveToFile(const std::string& filename) const {
    //     // Implementation to save reach constraints to file
    //     return true;
    // }

    // bool loadFromFile(const std::string& filename) {
    //     // Implementation to load reach constraints from file
    //     return true;
    // }

    bool saveToFile(const std::string& filename) const;

    bool loadFromFile(const std::string& filename);

// private:
    Map map_;
    Map grid_no_constraint;
    std::vector<Tunnel> tunnels_;
    std::map<int, TunnelGroup> tunnel_groups_;
    std::map<int, IntermediateGoalRegionperTunnel> intermediate_goal_regions_per_tunnel_;
    std::map<int, IntermediateGoalRegionperTunnelGroup> intermediate_goal_regions_per_tunnel_group_;
    double t_bound_1_;
    double t_bound_2_;
    std::map<int, std::vector<RootPathtoTunnelGroup>> root_paths_to_tunnel_groups_;
    std::map<int, std::vector<RootPathFromTunnelGroup>> root_paths_from_tunnel_groups_;
    // std::vector<IntermediateGoalRegionperTunnel> intermediate_goal_regions_;
    // std::vector<RootPathtoTunnelGroup> root_paths_to_tunnel_groups_;
    // std::vector<RootPathFromTunnelGroup> root_paths_from_tunnel_groups_;
    int perception_radius_ = 10; // Default perception radius


};

#endif // REACH_CONSTRAINT_H
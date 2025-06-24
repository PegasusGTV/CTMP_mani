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
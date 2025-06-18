#include "env_map.h"
#include<iostream>
#include<fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct Tunnel{
    int id;
    std::vector<Point> points;
    Point start;
};

struct TunnelGroup{
    int id;
    std::vector<Tunnel> Tunnels;
    Tunnel representative;
};

struct Path_to_solve_tunnel_constraints{
    int id;
    std::vector<Point> path;
};

class TunnelPreprocessor {
public:
    TunnelPreprocessor(const Map& map) : map_(map) {}

    void findTunnels(){
        int ids = 0;
        int y_length = map_.y_length;
        int tunnel_width = map_.tunnel_width;
        int d = 2*tunnel_width - 1;
        num_tunnels_in_group = tunnel_width;
        num_tunnels = y_length - tunnel_width + 1;
        std::vector<int> constraint_zone = map_.constraint_zone;
        int x_start = constraint_zone[0];
        for(int i = 0; i < num_tunnels; ++i) {
            Tunnel tunnel;
            tunnel.id = ids++;
            tunnel.start = Point(x_start, i + static_cast<int>(tunnel_width/2) + 1);
            for(int z = i; z < i + tunnel_width; ++z) {
                for (int j = constraint_zone[0]; j <= constraint_zone[1]; ++j) {
                    // if (i + j < y_length) {
                        tunnel.points.push_back(Point(j, z));
                    // }
                }
            }
            tunnels_.push_back(tunnel);
        }

    }

    void groupTunnels(){
        num_tunnel_groups = num_tunnels / num_tunnels_in_group;
        int ids = 0;
        for(int i = 0; i < num_tunnel_groups; ++i) {
            TunnelGroup group;
            group.id = ids++;
            for(int j = 0; j < num_tunnels_in_group; ++j) {
                group.Tunnels.push_back(tunnels_[i * num_tunnels_in_group + j]);
            }
            group.representative = group.Tunnels[static_cast<int>(num_tunnels_in_group/2) + 1];
            tunnel_groups_[group.id] = group;
        }
    }

    void solveTunnelConstraints() {
        int ids = 0;
        for (const auto& group : tunnel_groups_) {
            Path_to_solve_tunnel_constraints path;
            path.id = ids++;
            int x_start = group.second.representative.start.x;
            int y_start = group.second.representative.start.y;
            std::vector<std::pair<int, int>> tunnel_path = map_.tunnel_path;
            for (const auto& point : tunnel_path) {
                path.path.push_back(Point(point.first + x_start, point.second + y_start));
            }
            paths_to_solve_tunnel_constraints_[path.id] = path;
        }
    }

    bool saveTunnelsToFile(const std::string& filename) const {
        std::ofstream out{filename, std::ios::binary};
        if (!out) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return false;
        }   
    }


private:
    const Map& map_;
    std::vector<Tunnel> tunnels_;
    int num_tunnels_in_group;
    int num_tunnels;
    std::unordered_map<int, TunnelGroup> tunnel_groups_;
    int num_tunnel_groups;
    std::unordered_map<int, Path_to_solve_tunnel_constraints> paths_to_solve_tunnel_constraints_;
};
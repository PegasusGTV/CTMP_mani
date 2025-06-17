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

private:
    const Map& map_;
    std::vector<Tunnel> tunnels_;
    int num_tunnels_in_group;
    int num_tunnels;
};
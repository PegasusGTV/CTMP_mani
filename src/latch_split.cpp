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

private:
    const Map& map_;     
    
};
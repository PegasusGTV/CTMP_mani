#include<iostream>
#include<fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>

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

struct PointHash {
    std::size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
    }
};

struct Node {
    Point point;
    double g_cost; // Cost from start to this node
    double h_cost; // Heuristic cost to goal
    double f_cost(double weight = 1.0) const { return g_cost + weight * h_cost; }
    Node *parent; // Pointer to parent node

    Node(Point p, double g, double h, Node* par = nullptr)
        : point(p), g_cost(g), h_cost(h), parent(par) {}

    bool operator>(const Node& other) const {
        return f_cost() > other.f_cost();
    }
};


std::vector<Point> getNeighbors(const Point& point, const std::vector<std::vector<int>>& map) {
    std::vector<Point> neighbors;
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Right, Down, Left, Up
    for (const auto& dir : directions) {
        Point neighbor(point.x + dir[0], point.y + dir[1]);
        if (neighbor.x >= 0 && neighbor.x < map.size() &&
            neighbor.y >= 0 && neighbor.y < map[0].size() &&
            map[neighbor.x][neighbor.y] == 0) { // Assuming 0 is walkable
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
    
}

double heuristic(const Point& a, const Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Manhattan
}

std::vector<Point> WaStar(const std::vector<std::vector<int>>& map, const Point& start, const Point& goal, double weight) {
    auto cmp = [weight](Node* a, Node* b) { return a->f_cost(weight) > b->f_cost(weight); };
}
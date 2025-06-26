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
    double f_cost() const { return g_cost + h_cost; }
    Node *parent; // Pointer to parent node

    Node(Point p, double g, double h, Node* par = nullptr)
        : point(p), g_cost(g), h_cost(h), parent(par) {}

    bool operator>(const Node& other) const {
        return f_cost() > other.f_cost();
    }
};


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
    Point pos;
    double g_cost; // Cost from start to this node
    double h_cost; // Heuristic cost to goal
    double f_cost(double weight = 1.0) const { return g_cost + weight * h_cost; }
    Node *parent; // Pointer to parent node

    Node(Point p, double g, double h, Node* par = nullptr)
        : pos(p), g_cost(g), h_cost(h), parent(par) {}

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

std::vector<Point> WaStar(const std::vector<std::vector<int>>& grid, const Point& start, const Point& goal, double weight) {
    auto cmp = [weight](Node* a, Node* b) { return a->f_cost(weight) > b->f_cost(weight); };

    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open(cmp);
    std::unordered_map<Point, double, PointHash> g_score;
    std::unordered_map<Point, Node*, PointHash> all_nodes;

    Node* start_node = new Node(start, 0, heuristic(start, goal), nullptr);
    open.push(start_node);
    g_score[start] = 0;
    all_nodes[start] = start_node;

    while (!open.empty()) {
        Node* current = open.top(); open.pop();
        if (current->pos == goal) {
            std::vector<Point> path;
            for (Node* n = current; n != nullptr; n = n->parent)
                path.push_back(n->pos);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const Point& neighbor : getNeighbors(current->pos, grid)) {
            double tentative_g = current->g_cost + 1.0; // uniform cost
            if (!g_score.count(neighbor) || tentative_g < g_score[neighbor]) {
                g_score[neighbor] = tentative_g;
                Node* new_node = new Node(neighbor, tentative_g, heuristic(neighbor, goal), current);
                open.push(new_node);
                all_nodes[neighbor] = new_node;
            }
        }
    }

    std::cerr << "No path found.\n";
    return {};
}



int main() {
    std::vector<std::vector<int>> grid = {
        {0,0,0,0},
        {1,1,0,1},
        {0,0,0,0},
        {0,1,1,0}
    };
    Point start = {0, 0}, goal = {3, 3};
    double weight = 2.0;

    auto path = WaStar(grid, start, goal, weight);
    for (const auto& p : path)
        std::cout << "(" << p.x << ", " << p.y << ") ";
    std::cout << "\n";
}

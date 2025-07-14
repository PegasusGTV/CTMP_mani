#include "wastar.h"

std::vector<Point> getNeighbors(const Point& point, const std::vector<std::vector<int>>& map) {
    std::vector<Point> neighbors;
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Right, Down, Left, Up
    for (const auto& dir : directions) {
        Point neighbor(point.x + dir[0], point.y + dir[1]);
        if (neighbor.y >= 0 && neighbor.y < map.size() &&
            neighbor.x >= 0 && neighbor.x < map[0].size() &&
            map[neighbor.y][neighbor.x] == 0) { // Assuming 0 is walkable
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

    // std::cerr << "No path found.\n";
    return {};
}



// int main() {
//     std::vector<std::vector<int>> grid = {
//         {0,0,0,0},
//         {1,1,0,1},
//         {0,0,0,0},
//         {0,1,1,0}
//     };
//     Point start = {0, 0}, goal = {3, 3};
//     double weight = 2.0;

//     auto path = WaStar(grid, start, goal, weight);
//     for (const auto& p : path)
//         std::cout << "(" << p.x << ", " << p.y << ") ";
//     std::cout << "\n";
// }

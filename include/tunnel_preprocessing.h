#ifndef TUNNEL_PREPROCESSOR_H
#define TUNNEL_PREPROCESSOR_H
#pragma once
#include "env_map.h"                   // for Map
#include "point.h"                     // for Point
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>

/// A simple 2D integer point.
// struct Point {
//     int x, y;
//     Point();
//     Point(int x, int y);

//     bool operator==(const Point& other) const;
//     bool operator!=(const Point& other) const;
// };
// struct Point {
//     int x, y;
//     Point() : x(0), y(0) {}
//     Point(int x, int y) : x(x), y(y) {}
    
//     bool operator==(const Point& other) const {
//         return x == other.x && y == other.y;
//     }
    
//     bool operator!=(const Point& other) const {
//         return !(*this == other);
//     }

//     template<class Archive>
//     void serialize(Archive & ar, const unsigned int /*version*/) {
//         ar & x & y;
//     }
// };

/// A single tunnel: an ID, a list of coverage points, and a start.
struct Tunnel {
    int id;
    std::vector<Point> points;
    Point start;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & id & points & start;
    }
};

/// A group of tunnels sharing the same characteristics,
/// plus one tunnel chosen as representative.
struct TunnelGroup {
    int id;
    std::vector<Tunnel> Tunnels;
    Tunnel representative;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & id & Tunnels & representative;
    }
};

/// A precomputed path (identified by ID) for solving the
/// tunnel‐constraint problem in each tunnel group.
struct Path_to_solve_tunnel_constraints {
    int id;
    std::vector<Point> path;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
    ar & id & path;
    }
};

/// Preprocesses a grid‐map into tunnel‐groups and solves
/// their “tunnel‐constraint” paths once, offline. Provides
/// efficient binary I/O for the results.
class TunnelPreprocessor {
public:
    /// Build from a read‐only Map.
    explicit TunnelPreprocessor(const Map& map);

    /// Build the “raw” list of tunnels from map_.constraint_zone, etc.
    void findTunnels();

    /// Split tunnels_ into groups of width map_.tunnel_width.
    void groupTunnels();

    /// Solve each group’s representative tunnel exactly once.
    void solveTunnelConstraints();

    /// Write out all TunnelGroups + their representative solutions.
    /// Returns false on any I/O error.
    bool saveTunnelsToFile(const std::string& filename) const;

    /// Read back the exact same data. Returns false on any I/O error
    /// or file‐format inconsistency.
    bool loadTunnelsFromFile(const std::string& filename);

    std::vector<Tunnel> getTunnels();
// private:
    const Map& map_;  ///< source data

    // working data, built by findTunnels()/groupTunnels()/solveTunnelConstraints():
    std::vector<Tunnel>                                    tunnels_;
    int                                                    num_tunnels_in_group_;
    int                                                    num_tunnels_;
    // std::unordered_map<int, TunnelGroup>                   tunnel_groups_;
    std::map<int, TunnelGroup>                   tunnel_groups_;
    int                                                    num_tunnel_groups_;
    // std::unordered_map<int, Path_to_solve_tunnel_constraints>
    //                                         paths_to_solve_tunnel_constraints_;
    std::map<int, Path_to_solve_tunnel_constraints>
                                            paths_to_solve_tunnel_constraints_;
};

#endif  // TUNNEL_PREPROCESSOR_H

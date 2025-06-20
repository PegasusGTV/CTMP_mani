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
// };

// struct Tunnel{
//     int id;
//     std::vector<Point> points;
//     Point start;
// };

// struct TunnelGroup{
//     int id;
//     std::vector<Tunnel> Tunnels;
//     Tunnel representative;
// };

// struct Path_to_solve_tunnel_constraints{
//     int id;
//     std::vector<Point> path;
// };

// class TunnelPreprocessor {
// public:
//     TunnelPreprocessor(const Map& map) : map_(map) {}

//     void findTunnels(){
//         int ids = 0;
//         int y_length = map_.y_length;
//         int tunnel_width = map_.tunnel_width;
//         int d = 2*tunnel_width - 1;
//         num_tunnels_in_group = tunnel_width;
//         num_tunnels = y_length - tunnel_width + 1;
//         std::vector<int> constraint_zone = map_.constraint_zone;
//         int x_start = constraint_zone[0];
//         for(int i = 0; i < num_tunnels; ++i) {
//             Tunnel tunnel;
//             tunnel.id = ids++;
//             tunnel.start = Point(x_start, i + static_cast<int>(tunnel_width/2) + 1);
//             for(int z = i; z < i + tunnel_width; ++z) {
//                 for (int j = constraint_zone[0]; j <= constraint_zone[1]; ++j) {
//                     // if (i + j < y_length) {
//                         tunnel.points.push_back(Point(j, z));
//                     // }
//                 }
//             }
//             tunnels_.push_back(tunnel);
//         }

//     }

//     void groupTunnels(){
//         num_tunnel_groups = num_tunnels / num_tunnels_in_group;
//         int ids = 0;
//         for(int i = 0; i < num_tunnel_groups; ++i) {
//             TunnelGroup group;
//             group.id = ids++;
//             for(int j = 0; j < num_tunnels_in_group; ++j) {
//                 group.Tunnels.push_back(tunnels_[i * num_tunnels_in_group + j]);
//             }
//             group.representative = group.Tunnels[static_cast<int>(num_tunnels_in_group/2) + 1];
//             tunnel_groups_[group.id] = group;
//         }
//     }

//     void solveTunnelConstraints() {
//         int ids = 0;
//         for (const auto& group : tunnel_groups_) {
//             Path_to_solve_tunnel_constraints path;
//             path.id = ids++;
//             int x_start = group.second.representative.start.x;
//             int y_start = group.second.representative.start.y;
//             std::vector<std::pair<int, int>> tunnel_path = map_.tunnel_path;
//             for (const auto& point : tunnel_path) {
//                 path.path.push_back(Point(point.first + x_start, point.second + y_start));
//             }
//             paths_to_solve_tunnel_constraints_[path.id] = path;
//         }
//     }

//     bool saveTunnelsToFile(const std::string& filename) const {
//         std::ofstream out{filename, std::ios::binary};
//         if (!out) {
//             std::cerr << "Error opening file for writing: " << filename << std::endl;
//             return false;
//         }
        
//         auto write = [&](auto &v){
//             out.write(reinterpret_cast<const char*>(&v), sizeof(v));
//         };

//         int32_t G = int32_t(tunnel_groups_.size());
//         write(G);

//         for(auto const &kv : tunnel_groups_){
//             auto const &group = kv.second;

//             int32_t gid = int32_t(group.id);
//             write(gid);

//             int32_t T = int32_t(group.Tunnels.size());
//             write(T);

//             for(auto const &tun : group.Tunnels){
//                 int32_t tid = int32_t(tun.id);
//                 write(tid);

//                 int32_t P = int32_t(tun.points.size());
//                 write(P);

//                 for(const auto &point : tun.points){
//                     write(point.x);
//                     write(point.y);
//                 }

//                 write(tun.start.x);
//                 write(tun.start.y);
//             }

//             int32_t rep_id = int32_t(group.representative.id);
//             write(rep_id);
//         }

//         int32_t P = int32_t(paths_to_solve_tunnel_constraints_.size());
//         write(P);
//         for(auto const &kv : paths_to_solve_tunnel_constraints_){
//             auto const &path = kv.second;

//             int32_t pid = int32_t(path.id);
//             write(pid);

//             int32_t L = int32_t(path.path.size());
//             write(L);

//             for(const auto &point : path.path){
//                 write(point.x);
//                 write(point.y);
//             }
//         }

//         out.close();
//         return !out.fail();

//     }

//     bool loadTunnelsFromFile(const std::string& filename) {
//         std::ifstream in{filename, std::ios::binary};
//         if (!in) {
//             std::cerr << "Error opening file for reading: " << filename << std::endl;
//             return false;
//         }
//         // helper to read POD types
//         auto read = [&](auto &v){
//             in.read(reinterpret_cast<char*>(&v), sizeof(v));
//         };
    
//         // clear existing
//         tunnel_groups_.clear();
//         paths_to_solve_tunnel_constraints_.clear();
    
//         // 1) number of groups
//         int32_t G;
//         read(G);
//         for(int gi=0; gi<G; ++gi){
//             TunnelGroup group;
//             // group id
//             read(group.id);
//             // number of tunnels in this group
//             int32_t T;
//             read(T);
//             group.Tunnels.reserve(T);
    
//             // read each Tunnel
//             for(int ti=0; ti<T; ++ti){
//                 Tunnel t;
//                 read(t.id);
//                 int32_t P;
//                 read(P);
//                 t.points.resize(P);
//                 for(int pi=0; pi<P; ++pi){
//                     read(t.points[pi].x);
//                     read(t.points[pi].y);
//                 }
//                 read(t.start.x);
//                 read(t.start.y);
//                 group.Tunnels.push_back(std::move(t));
//             }
    
//             // read representative tunnel id, then find it in the just-read list
//             int32_t rep_id;
//             read(rep_id);
//             auto it = std::find_if(group.Tunnels.begin(),
//                                    group.Tunnels.end(),
//                                    [&](auto &tun){ return tun.id == rep_id; });
//             if(it == group.Tunnels.end()){
//                 std::cerr << "Corrupt file: representative id " << rep_id
//                           << " not in group " << group.id << std::endl;
//                 return false;
//             }
//             group.representative = *it;
    
//             tunnel_groups_.emplace(group.id, std::move(group));
//         }
    
//         // 2) number of paths
//         int32_t P;
//         read(P);
//         for(int pi=0; pi<P; ++pi){
//             Path_to_solve_tunnel_constraints path;
//             read(path.id);
//             int32_t L;
//             read(L);
//             path.path.resize(L);
//             for(int i=0; i<L; ++i){
//                 read(path.path[i].x);
//                 read(path.path[i].y);
//             }
//             paths_to_solve_tunnel_constraints_.emplace(path.id, std::move(path));
//         }
    
//         return !in.fail();
//     }


// private:
//     const Map& map_;
//     std::vector<Tunnel> tunnels_;
//     int num_tunnels_in_group;
//     int num_tunnels;
//     std::unordered_map<int, TunnelGroup> tunnel_groups_;
//     int num_tunnel_groups;
//     std::unordered_map<int, Path_to_solve_tunnel_constraints> paths_to_solve_tunnel_constraints_;
// };






TunnelPreprocessor::TunnelPreprocessor(const Map& map) : map_(map) {}

void TunnelPreprocessor::findTunnels(){
    int ids = 0;
    int y_length = map_.y_length;
    int tunnel_width = map_.tunnel_width;
    int d = 2*tunnel_width - 1;
    num_tunnels_in_group_ = tunnel_width;
    num_tunnels_ = y_length - tunnel_width + 1;
    std::vector<int> constraint_zone = map_.constraint_zone;
    int x_start = constraint_zone[0];
    for(int i = 0; i < num_tunnels_; ++i) {
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

void TunnelPreprocessor::groupTunnels(){
    num_tunnel_groups_ = num_tunnels_ / num_tunnels_in_group_;
    int ids = 0;
    for(int i = 0; i < num_tunnel_groups_; ++i) {
        TunnelGroup group;
        group.id = ids++;
        for(int j = 0; j < num_tunnels_in_group_; ++j) {
            group.Tunnels.push_back(tunnels_[i * num_tunnels_in_group_ + j]);
        }
        group.representative = group.Tunnels[static_cast<int>(num_tunnels_in_group_/2) + 1];
        tunnel_groups_[group.id] = group;
    }
}

void TunnelPreprocessor::solveTunnelConstraints() {
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

// bool TunnelPreprocessor::saveTunnelsToFile(const std::string& filename) const {
//     std::ofstream out{filename, std::ios::binary};
//     if (!out) {
//         std::cerr << "Error opening file for writing: " << filename << std::endl;
//         return false;
//     }
    
//     auto write = [&](auto &v){
//         out.write(reinterpret_cast<const char*>(&v), sizeof(v));
//     };

//     int32_t G = int32_t(tunnel_groups_.size());
//     write(G);

//     for(auto const &kv : tunnel_groups_){
//         auto const &group = kv.second;

//         int32_t gid = int32_t(group.id);
//         write(gid);

//         int32_t T = int32_t(group.Tunnels.size());
//         write(T);

//         for(auto const &tun : group.Tunnels){
//             int32_t tid = int32_t(tun.id);
//             write(tid);

//             int32_t P = int32_t(tun.points.size());
//             write(P);

//             for(const auto &point : tun.points){
//                 write(point.x);
//                 write(point.y);
//             }

//             write(tun.start.x);
//             write(tun.start.y);
//         }

//         int32_t rep_id = int32_t(group.representative.id);
//         write(rep_id);
//     }

//     int32_t P = int32_t(paths_to_solve_tunnel_constraints_.size());
//     write(P);
//     for(auto const &kv : paths_to_solve_tunnel_constraints_){
//         auto const &path = kv.second;

//         int32_t pid = int32_t(path.id);
//         write(pid);

//         int32_t L = int32_t(path.path.size());
//         write(L);

//         for(const auto &point : path.path){
//             write(point.x);
//             write(point.y);
//         }
//     }

//     out.close();
//     return !out.fail();

// }

// bool TunnelPreprocessor::loadTunnelsFromFile(const std::string& filename) {
//     std::ifstream in{filename, std::ios::binary};
//     if (!in) {
//         std::cerr << "Error opening file for reading: " << filename << std::endl;
//         return false;
//     }
//     // helper to read POD types
//     auto read = [&](auto &v){
//         in.read(reinterpret_cast<char*>(&v), sizeof(v));
//     };

//     // clear existing
//     tunnel_groups_.clear();
//     paths_to_solve_tunnel_constraints_.clear();

//     // 1) number of groups
//     int32_t G;
//     read(G);
//     for(int gi=0; gi<G; ++gi){
//         TunnelGroup group;
//         // group id
//         read(group.id);
//         // number of tunnels in this group
//         int32_t T;
//         read(T);
//         group.Tunnels.reserve(T);

//         // read each Tunnel
//         for(int ti=0; ti<T; ++ti){
//             Tunnel t;
//             read(t.id);
//             int32_t P;
//             read(P);
//             t.points.resize(P);
//             for(int pi=0; pi<P; ++pi){
//                 read(t.points[pi].x);
//                 read(t.points[pi].y);
//             }
//             read(t.start.x);
//             read(t.start.y);
//             group.Tunnels.push_back(std::move(t));
//         }

//         // read representative tunnel id, then find it in the just-read list
//         int32_t rep_id;
//         read(rep_id);
//         auto it = std::find_if(group.Tunnels.begin(),
//                                 group.Tunnels.end(),
//                                 [&](auto &tun){ return tun.id == rep_id; });
//         if(it == group.Tunnels.end()){
//             std::cerr << "Corrupt file: representative id " << rep_id
//                         << " not in group " << group.id << std::endl;
//             return false;
//         }
//         group.representative = *it;

//         tunnel_groups_.emplace(group.id, std::move(group));
//     }

//     // 2) number of paths
//     int32_t P;
//     read(P);
//     for(int pi=0; pi<P; ++pi){
//         Path_to_solve_tunnel_constraints path;
//         read(path.id);
//         int32_t L;
//         read(L);
//         path.path.resize(L);
//         for(int i=0; i<L; ++i){
//             read(path.path[i].x);
//             read(path.path[i].y);
//         }
//         paths_to_solve_tunnel_constraints_.emplace(path.id, std::move(path));
//     }

//     return !in.fail();
// }

bool TunnelPreprocessor::saveTunnelsToFile(const std::string& filename) const {
    try {
      boost::filesystem::ofstream ofs(filename);
      boost::archive::text_oarchive oa(ofs);
      oa << tunnels_
         << num_tunnels_in_group_
         << num_tunnels_
         << num_tunnel_groups_
         << tunnel_groups_
         << paths_to_solve_tunnel_constraints_;
      return true;
    }
    catch (std::exception &e) {
      std::cerr << "Save failed: " << e.what() << std::endl;
      return false;
    }
        
}

bool TunnelPreprocessor::loadTunnelsFromFile(const std::string& filename) {
    try {
        boost::filesystem::ifstream ifs(filename);
        boost::archive::text_iarchive ia(ifs);
        ia >> tunnels_
           >> num_tunnels_in_group_
           >> num_tunnels_
           >> num_tunnel_groups_
           >> tunnel_groups_
           >> paths_to_solve_tunnel_constraints_;
        return true;
    }
    catch (std::exception &e) {
        std::cerr << "Load failed: " << e.what() << std::endl;
        return false;
    }
}
std::vector<Tunnel> TunnelPreprocessor::getTunnels() {
    return tunnels_;
}
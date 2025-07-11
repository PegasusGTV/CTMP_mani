#include "env_map.h"
#include "tunnel_preprocessing.h"
#include "reach_constraint.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


// namespace fs = std::filesystem;

int main() {
  const std::string map_txt  = "../maps/map.txt";
  const std::string map_meta = "../maps/map_meta.json";
  const std::string out_bin  = "../data/test_tunnels.dat";
  const std::string rc_file  = "../data/reach_constraint.dat";
  const std::string out_json  = "../data/test_tunnels.json";

  try {
    // 1) Load
    Map env = loadMapFromFiles_preprocessing(map_txt, map_meta);
    std::cout << "Map loaded: "
              << env.x_length << "×" << env.y_length
              << "  start=(" << env.start.first  << "," << env.start.second << ")"
              << "  goal =(" << env.goal.first   << "," << env.goal.second  << ")\n";

    // 2) Preprocess
    TunnelPreprocessor pre(env);
    pre.findTunnels();
    pre.groupTunnels();
    pre.solveTunnelConstraints();
    // pre.saveTunnelsToFile(out_bin);

    ReachConstraint rc(env);
    rc.findIntermediateGoalRegions(pre.tunnels_, pre.tunnel_groups_, 2, 2);  // Adjust t_bounds as needed
    std::cout<<"part 1"<<"\n";
    // rc.findRootPathsToTunnelGroups();
    // std::cout<<"part 2"<<"\n";
    // rc.findRootPathsFromTunnelGroups();
    // std::cout<<"part 3"<<"\n";

    // 3) Print discovered tunnels
    const auto &tunnels = pre.getTunnels();
    const auto &path = pre.paths_to_solve_tunnel_constraints_;
    // std::cout << "Discovered " << tunnels.size() << " tunnels:\n";
    // for (auto &t : tunnels) {
    //   std::cout << "  Tunnel #" << t.id
    //             << "  length=" << t.points.size() << " cells\n";
    // }

    // std::cout<< "number of tunnel groups: " << pre.num_tunnels_in_group_ << "\n";
    // std::cout<< "number of tunnels: " << pre.num_tunnels_ << "\n";
    // std::cout<< "number of tunnel groups: " << pre.num_tunnel_groups_ << "\n";

    // 4) Save to disk
    if (!pre.saveTunnelsToFile(out_bin)) {
      throw std::runtime_error("Failed to save tunnels to " + out_bin);
    }
    std::cout << "Saved tunnel data to '" << out_bin << "'.\n";


    // {
    //   json j;
    //   j["map"] = {
    //     {"width",  env.x_length},
    //     {"height", env.y_length},
    //     {"start", {{"x", env.start.first}, {"y", env.start.second}}},
    //     {"goal",  {{"x", env.goal.first},  {"y", env.goal.second}}}
    //   };
    //   j["tunnels"] = json::array();
    //   for (auto const &t : tunnels) {
    //     json tj;
    //     tj["id"]     = t.id;
    //     tj["points"] = json::array();
    //     for (auto const &p : t.points) {
    //       tj["points"].push_back({{"x", p.x}, {"y", p.y}});
    //     }
    //     j["tunnels"].push_back(std::move(tj));
    //   }

    //   j["paths"] = json::array();
    //   for (auto const & kv : path)
    //   {
    //   const auto & path_struct = kv.second;
    //   json pj;
    //   pj["id"]     = path_struct.id;
    //   pj["points"] = json::array();
    //   for (auto const & p : path_struct.path)
    //   {
    //       pj["points"].push_back({
    //       { "x", p.x },
    //       { "y", p.y }
    //       });
    //   }
    //   j["paths"].push_back(std::move(pj));
    //   }

    //   std::ofstream ofs(out_json);
    //   if (!ofs) throw std::runtime_error("Could not open " + out_json);
    //   ofs << j.dump(2) << std::endl;
    //   std::cout << "Also wrote tunnels JSON to '" << out_json << "'.\n";
    // }

    // 5) Reload and verify
    TunnelPreprocessor reloaded(env);
    if (!reloaded.loadTunnelsFromFile(out_bin)) {
      throw std::runtime_error("Failed to load tunnels from " + out_bin);
    }

    std::cout << "Reloaded " << reloaded.getTunnels().size() << " tunnels successfully.\n";

    std::cout<< "number of tunnel groups: " << reloaded.num_tunnels_in_group_ << "\n";
    std::cout<< "number of tunnels: " << reloaded.num_tunnels_ << "\n";
    std::cout<< "number of tunnel groups: " << reloaded.num_tunnel_groups_ << "\n";





    if (!rc.saveToFile(rc_file)) {
      throw std::runtime_error("Failed to save ReachConstraint to " + rc_file);
    }

    ReachConstraint rc_reloaded(env);
    if (!rc_reloaded.loadFromFile(rc_file)) {
      throw std::runtime_error("Failed to load ReachConstraint from " + rc_file);
    }

    std::cout << "Reach constraints saved and reloaded successfully.\n";

    return 0;
  }
  catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}

// #include "env_map.h"
// #include "tunnel_preprocessing.h"
// #include "reach_constraint.h"
// #include <iostream>
// #include <filesystem>



// int main() {
//   const std::string map_txt  = "../maps/map.txt";
//   const std::string map_meta = "../maps/map_meta.json";
//   const std::string out_bin  = "../data/test_tunnels.dat";
  // const std::string rc_file  = "../data/reach_constraint.dat";

//   try {
//     // 1) Load the map
//     Map env = loadMapFromFiles_preprocessing(map_txt, map_meta);
//     std::cout << "Map loaded: "
//               << env.x_length << "×" << env.y_length
//               << "  start=(" << env.start.first  << "," << env.start.second << ")"
//               << "  goal =(" << env.goal.first   << "," << env.goal.second  << ")\n";

//     // 2) Preprocess tunnels
//     TunnelPreprocessor pre(env);
//     pre.findTunnels();
//     pre.groupTunnels();
//     pre.solveTunnelConstraints();

//     const auto &tunnels = pre.tunnels_;
//     const auto &tunnel_groups = pre.tunnel_groups_;

//     std::cout << "Discovered " << tunnels.size() << " tunnels.\n";
//     std::cout << "Number of tunnel groups: " << tunnel_groups.size() << "\n";

//     // 3) Save tunnel data
//     if (!pre.saveTunnelsToFile(out_bin)) {
//       throw std::runtime_error("Failed to save tunnels to " + out_bin);
//     }

//     // 4) Compute reach constraints
//     ReachConstraint rc(env);
//     std::cout << "Computing reach constraints...\n";
//     rc.findIntermediateGoalRegions(tunnels, tunnel_groups, 1, 2);  // Adjust t_bounds as needed
//     std::cout<< "part 1";
//     rc.findRootPathsToTunnelGroups();
//     std::cout<< "part 2";
//     rc.findRootPathsFromTunnelGroups();

//     // 5) Save and reload reach constraints
//     if (!rc.saveToFile(rc_file)) {
//       throw std::runtime_error("Failed to save ReachConstraint to " + rc_file);
//     }

//     ReachConstraint rc_reloaded(env);
//     if (!rc_reloaded.loadFromFile(rc_file)) {
//       throw std::runtime_error("Failed to load ReachConstraint from " + rc_file);
//     }

//     std::cout << "Reach constraints saved and reloaded successfully.\n";
//     return 0;
//   }
//   catch (const std::exception &e) {
//     std::cerr << "Error: " << e.what() << "\n";
//     return 1;
//   }
// }


// #include "reach_constraint.h"
// #include "tunnel_preprocessing.h"
// #include <iostream>
// #include <fstream>

// int main() {
//     // Dummy 10x10 map
//     Map map;
//     map.x_length = 10;
//     map.y_length = 10;
//     map.start = {0, 5};
//     map.goal = {9, 5};
//     map.constraint_zone = {3, 7};
//     map.intermediate_goal_zone = {2, 6};
//     map.tunnel_width = 2;
//     map.occupancy_grid = std::vector<std::vector<int>>(10, std::vector<int>(10, 0)); // All free

//     Tunnel tunnel;
//     tunnel.id = 1;
//     tunnel.start = {5, 5};
//     tunnel.points = {{5,5}, {5,6}, {5,7}};

//     std::vector<Tunnel> tunnels = {tunnel};

//     TunnelGroup group;
//     group.id = 10;
//     group.representative = tunnel;
//     group.Tunnels = {tunnel};

//     std::map<int, TunnelGroup> tunnel_groups = {{10, group}};

//     ReachConstraint reach(map);
//     reach.findIntermediateGoalRegions(tunnels, tunnel_groups, 0.01, 0.01);
//     reach.findRootPathsToTunnelGroups();
//     reach.findRootPathsFromTunnelGroups();

//     // std::string filename = "reach_test_save.dat";

//     // if (reach.saveToFile(filename)) {
//     //     std::cout << "Saved ReachConstraint successfully.\n";
//     // } else {
//     //     std::cerr << "Failed to save ReachConstraint.\n";
//     // }

//     // ReachConstraint loaded_reach(map);
//     // if (loaded_reach.loadFromFile(filename)) {
//     //     std::cout << "Loaded ReachConstraint successfully.\n";
//     // } else {
//     //     std::cerr << "Failed to load ReachConstraint.\n";
//     // }

//     std::cout << "=== REACH CONSTRAINT INTERNAL STATE ===\n";

//     // 1) How many intermediate goal regions did we carve out?
//     const auto& igr = reach.intermediate_goal_regions_per_tunnel_group_;  
//     std::cout << "IntermediateGoalRegions.size() = " << igr.size() << "\n";
//     for(auto& p: igr){
//     //   std::cout << "  Region["<<i<<"] := ";
//       std::cout << "("<<p.first<<","<<p.second.tunnel_group_id<<") ";
//       std::cout<<"\n";
//     }

//     // 2) How many root‐to‐tunnel‐group paths?
//     const auto& R2TG = reach.root_paths_to_tunnel_groups_;
//     std::cout << "RootPathsToTunnelGroups.size() = " << R2TG.size() << "\n";
//     for(auto const& [tgid, path] : R2TG){
//       std::cout << "  TunnelGroup " << tgid
//                 << " : path length = " << path.size() << "\n";
//     }

//     // 3) How many root‐from‐tunnel‐group paths?
//     const auto& RfTG = reach.root_paths_from_tunnel_groups_;
//     std::cout << "RootPathsFromTunnelGroups.size() = " << RfTG.size() << "\n";
//     for(auto const& [tgid, path] : RfTG){
//       std::cout << "  TunnelGroup " << tgid
//                 << " : path length = " << path.size() << "\n";
//     }
//     std::cout << "========================================\n";

//     // now test serialization
//     std::string filename = "reach_test_save.dat";
//     if (reach.saveToFile(filename)) {
//         std::cout << "Saved ReachConstraint successfully.\n";
//     } else {
//         std::cerr << "Failed to save ReachConstraint.\n";
//     }

//     ReachConstraint loaded_reach(map);
//     if (loaded_reach.loadFromFile(filename)) {
//         std::cout << "Loaded ReachConstraint successfully.\n";
//     } else {
//         std::cerr << "Failed to load ReachConstraint.\n";
//     }


//     return 0;
// }
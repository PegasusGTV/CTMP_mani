#include "env_map.h"
#include "tunnel_preprocessing.h"
#include <iostream>
#include <filesystem>

// namespace fs = std::filesystem;

int main() {
  const std::string map_txt  = "../maps/map.txt";
  const std::string map_meta = "../maps/map_meta.json";
  const std::string out_bin  = "../data/test_tunnels.dat";

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
    pre.solveTunnelConstraints();\
    // pre.saveTunnelsToFile(out_bin);

    // 3) Print discovered tunnels
    const auto &tunnels = pre.getTunnels();
    std::cout << "Discovered " << tunnels.size() << " tunnels:\n";
    for (auto &t : tunnels) {
      std::cout << "  Tunnel #" << t.id
                << "  length=" << t.points.size() << " cells\n";
    }

    std::cout<< "number of tunnel groups: " << pre.num_tunnels_in_group_ << "\n";
    std::cout<< "number of tunnels: " << pre.num_tunnels_ << "\n";
    std::cout<< "number of tunnel groups: " << pre.num_tunnel_groups_ << "\n";

    // 4) Save to disk
    if (!pre.saveTunnelsToFile(out_bin)) {
      throw std::runtime_error("Failed to save tunnels to " + out_bin);
    }
    std::cout << "Saved tunnel data to '" << out_bin << "'.\n";

    // 5) Reload and verify
    TunnelPreprocessor reloaded(env);
    if (!reloaded.loadTunnelsFromFile(out_bin)) {
      throw std::runtime_error("Failed to load tunnels from " + out_bin);
    }

    std::cout << "Reloaded " << reloaded.getTunnels().size() << " tunnels successfully.\n";

    std::cout<< "number of tunnel groups: " << reloaded.num_tunnels_in_group_ << "\n";
    std::cout<< "number of tunnels: " << reloaded.num_tunnels_ << "\n";
    std::cout<< "number of tunnel groups: " << reloaded.num_tunnel_groups_ << "\n";

    return 0;
  }
  catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
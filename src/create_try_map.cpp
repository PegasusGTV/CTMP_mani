#include "env_map.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <random>
using json = nlohmann::json;

int main(){
    const std::string map_txt  = "../maps/map.txt";
    const std::string map_meta = "../maps/map_meta.json";

    std::mt19937 rng(std::random_device{}());

    Map Online_map = loadMapFromFiles_preprocessing(map_txt, map_meta);

    // int x_length = Online_map.x_length;

    int number_of_tunnels = Online_map.x_length - Online_map.tunnel_width + 1;
    // std::cout << "Number of tunnels: " << number_of_tunnels << "\n";
    int num_of_groups = number_of_tunnels/Online_map.tunnel_width;

    std::uniform_int_distribution<int> dist(0, number_of_tunnels - 1);

    int choice = dist(rng);

}
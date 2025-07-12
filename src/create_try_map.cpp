#include "env_map.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main(){
    const std::string map_txt  = "../maps/map.txt";
    const std::string map_meta = "../maps/map_meta.json";

    Map Online_map = loadMapFromFiles_preprocessing(map_txt, map_meta);
    
}
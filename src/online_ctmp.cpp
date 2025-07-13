#include "point.h"
#include "env_map.h"
#include "reach_constraint.h"
#include "tunnel_preprocessing.h"

int main(){
    const std::string map_txt  = "../maps/map.txt";
    const std::string map_meta = "../maps/map_meta.json";
    const std::string out_json  = "../maps/online_data.json";
    const std::string out_bin  = "../data/test_tunnels.dat";
    const std::string rc_file  = "../data/reach_constraint.dat";

    Map env = loadMapFromFiles_preprocessing(map_txt, map_meta);
    



    return 0;
}
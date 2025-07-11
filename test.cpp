#include <iostream>
#include <vector>

int main(){
    for(int i = 0; i < 6; ++i) {
        std::cout << static_cast<int>(i/2) << std::endl;
    }
}
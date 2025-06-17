cmake_minimum_required(VERSION 3.10)
project(CTMP_MANI)

# — Require C++17 —
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# — Collect all your .cpp files under src/ —
file(GLOB_RECURSE SRC_FILES
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# — Declare the executable —
add_executable(ctmp_mani ${SRC_FILES})

# — Make sure our own headers are visible — 
#    this lets you #include "env_map.h" or #include <nlohmann/json.hpp>
#    from include/nlohmann/json.hpp
target_include_directories(ctmp_mani PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

# — Try to find a system-installed nlohmann_json — 
find_package(nlohmann_json 3.2.0 QUIET)

if (nlohmann_json_FOUND)
  message(STATUS "Using system nlohmann_json v${nlohmann_json_VERSION}")
  target_link_libraries(ctmp_mani PRIVATE nlohmann_json::nlohmann_json)
else()
  message(STATUS "system nlohmann_json not found, using bundled include/nlohmann/json.hpp")
  # nothing else to do — the single‐header in include/nlohmann/json.hpp will be picked up
endif()

# (later, if you add other libs:)
# target_link_libraries(ctmp_mani PRIVATE other_library)

cmake_minimum_required(VERSION 3.10)
project(ctmp_mani LANGUAGES CXX)

# ─── C++ standard ────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── include paths ───────────────────────────────────────────────────────────
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/search/include
)

# ─── find external deps ──────────────────────────────────────────────────────
find_package(Boost REQUIRED COMPONENTS serialization)
find_package(nlohmann_json QUIET)

# ─── build a “core” library from your src/ and search/src/ ────────────────────
file(GLOB_RECURSE CTMP_CORE_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/search/src/*.cpp
)
# strip out all test_*.cpp so they don’t get lumped into the core
list(FILTER CTMP_CORE_SOURCES
     EXCLUDE REGEX ".*/test_.*\\.cpp$")

add_library(ctmp_core STATIC ${CTMP_CORE_SOURCES})

target_link_libraries(ctmp_core
  PUBLIC
    Boost::serialization
  $<$<BOOL:${nlohmann_json_FOUND}>:nlohmann_json::nlohmann_json>
)

# ─── test: reach_constraint ──────────────────────────────────────────────────
add_executable(test_reach_constraint
  ${PROJECT_SOURCE_DIR}/src/test_reach_constraint.cpp
  ${PROJECT_SOURCE_DIR}/src/load_map_preprocessing.cpp
  ${PROJECT_SOURCE_DIR}/src/latch_split.cpp
  ${PROJECT_SOURCE_DIR}/src/reach_constraint.cpp
  ${PROJECT_SOURCE_DIR}/search/src/wastar.cpp
)
target_link_libraries(test_reach_constraint PRIVATE ctmp_core)

# ─── test: map loader ─────────────────────────────────────────────────────────
add_executable(test_map
  ${PROJECT_SOURCE_DIR}/src/test_map.cpp
  ${PROJECT_SOURCE_DIR}/src/load_map_preprocessing.cpp
)
target_link_libraries(test_map PRIVATE ctmp_core)

# ─── (OPTIONAL) your main executable ─────────────────────────────────────────
# Once you have a real “main” file, uncomment and point to it:
#
# add_executable(ctmp_mani
#   ${PROJECT_SOURCE_DIR}/src/your_real_main.cpp
# )
# target_link_libraries(ctmp_mani PRIVATE ctmp_core)

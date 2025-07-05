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

# link Boost.Serialization (and JSON if you want it globally)
target_link_libraries(ctmp_core
  PUBLIC
    Boost::serialization
  $<$<BOOL:${nlohmann_json_FOUND}>:nlohmann_json::nlohmann_json>
)

# ─── your main executable ────────────────────────────────────────────────────
# you didn’t show a main.cpp, so I’m assuming ctmp_mani.cpp or similar
# if you have one, replace “apps/main.cpp” with the correct path
add_executable(ctmp_mani
  ${PROJECT_SOURCE_DIR}/src/ctmp_mani.cpp  # ← your real “main”
)
target_link_libraries(ctmp_mani PRIVATE ctmp_core)

# ─── test: reach_constraint ──────────────────────────────────────────────────
add_executable(test_reach_constraint
  ${PROJECT_SOURCE_DIR}/src/test_reach_constraint.cpp
  ${PROJECT_SOURCE_DIR}/src/load_map_preprocessing.cpp
  ${PROJECT_SOURCE_DIR}/src/latch_split.cpp
  ${PROJECT_SOURCE_DIR}/src/reach_constraint.cpp
  ${PROJECT_SOURCE_DIR}/search/src/wastar.cpp
)
target_link_libraries(test_reach_constraint PRIVATE
  ctmp_core
)

# ─── test: map loader ─────────────────────────────────────────────────────────
add_executable(test_map
  ${PROJECT_SOURCE_DIR}/src/test_map.cpp
  ${PROJECT_SOURCE_DIR}/src/load_map_preprocessing.cpp
)
target_link_libraries(test_map PRIVATE
  ctmp_core
)

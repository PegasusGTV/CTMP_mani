cmake_minimum_required(VERSION 3.10)
project(ctmp_mani LANGUAGES CXX)

# ─── C++ standard ────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# ─── point ctmp_core at your headers ─────────────────────────────────────────
# this replaces the old include_directories(...) call
target_include_directories(ctmp_core
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/search/include>
)

# ─── test: reach_constraint ──────────────────────────────────────────────────
add_executable(test_reach_constraint
  src/test_reach_constraint.cpp
  src/load_map_preprocessing.cpp
  src/latch_split.cpp
  src/reach_constraint.cpp
  search/src/wastar.cpp
)
target_link_libraries(test_reach_constraint PRIVATE ctmp_core)

# ─── test: map loader ─────────────────────────────────────────────────────────
add_executable(test_map
  src/test_map.cpp
  src/load_map_preprocessing.cpp
)
target_link_libraries(test_map PRIVATE ctmp_core)

# ─── your own “main” ─────────────────────────────────────────────────────────
# here’s how you add create_try_map.cpp as an exe
add_executable(create_try_map
  src/create_try_map.cpp
  src/load_map_preprocessing.cpp    # if that’s needed
  # … any other .cpp that it depends on …
)
target_link_libraries(create_try_map PRIVATE ctmp_core)

# ─── (OPTIONAL) other mains ──────────────────────────────────────────────────
add_executable(online_ctmp
  src/online_ctmp.cpp
)
target_link_libraries(online_ctmp PRIVATE ctmp_core)

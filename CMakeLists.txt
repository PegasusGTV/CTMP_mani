cmake_minimum_required(VERSION 3.10)
project(ctmp_mani LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------
# where to find your headers
# ----------------------------------------------------------------------------
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/search/include
)

# ----------------------------------------------------------------------------
# all of your “library” sources, but EXCLUDE the test’s .cpp
# ----------------------------------------------------------------------------
file(GLOB_RECURSE CTMP_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/search/src/*.cpp
)
# remove the test file from the main build
list(FILTER CTMP_SOURCES EXCLUDE REGEX ".*/test_reach_constraint\\.cpp$")

# ----------------------------------------------------------------------------
# main executable
# ----------------------------------------------------------------------------
add_executable(ctmp_mani
  ${CTMP_SOURCES}
)

# ----------------------------------------------------------------------------
# test executable
# ----------------------------------------------------------------------------
add_executable(test_reach_constraint
  src/test_reach_constraint.cpp
  src/load_map_preprocessing.cpp
  src/latch_split.cpp
  src/reach_constraint.cpp
  search/src/wastar.cpp
)

# ----------------------------------------------------------------------------
# link Boost.Serialization
# ----------------------------------------------------------------------------
find_package(Boost REQUIRED COMPONENTS serialization)
if(Boost_FOUND)
  target_include_directories(ctmp_mani           PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries   (ctmp_mani           PRIVATE ${Boost_LIBRARIES})

  target_include_directories(test_reach_constraint PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries   (test_reach_constraint PRIVATE ${Boost_LIBRARIES})
endif()

# ----------------------------------------------------------------------------
# optional: JSON
# ----------------------------------------------------------------------------
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
  target_link_libraries(ctmp_mani            PRIVATE nlohmann_json::nlohmann_json)
  target_link_libraries(test_reach_constraint PRIVATE nlohmann_json::nlohmann_json)
endif()

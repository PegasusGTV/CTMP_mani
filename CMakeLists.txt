cmake_minimum_required(VERSION 3.10)
project(ctmp_mani LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------
# 1) Main executable and sources
# ----------------------------------------------------------------------------

# Grab all .cpp files under src/
file(GLOB_RECURSE CTMP_SRC CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(ctmp_mani
  ${CTMP_SRC}
)

# Include headers from include/
target_include_directories(ctmp_mani PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

# ----------------------------------------------------------------------------
# 2) Boost Serialization
# ----------------------------------------------------------------------------

find_package(Boost REQUIRED COMPONENTS serialization)
if (Boost_FOUND)
  message(STATUS "Found Boost: ${Boost_INCLUDE_DIRS}")
  target_include_directories(ctmp_mani PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries(ctmp_mani PRIVATE ${Boost_LIBRARIES})
else()
  message(FATAL_ERROR "Boost.Serialization not found")
endif()

# ----------------------------------------------------------------------------
# 3) Optional: system nlohmann_json
# ----------------------------------------------------------------------------

find_package(nlohmann_json QUIET)
if (nlohmann_json_FOUND)
  message(STATUS "Using system nlohmann_json")
  target_link_libraries(ctmp_mani PRIVATE
    nlohmann_json::nlohmann_json
  )
else()
  message(STATUS "System nlohmann_json not found, using bundled one in include/")
endif()

# ----------------------------------------------------------------------------
# 4) Optional: SBPL or other libraries
# ----------------------------------------------------------------------------
# find_package(SBPL REQUIRED)
# target_link_libraries(ctmp_mani PRIVATE sbpl)

# ----------------------------------------------------------------------------
# 5) Tests
# ----------------------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" OFF)

if (BUILD_TESTS)
  enable_testing()
  file(GLOB TEST_SRC CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/tests/*.cpp
  )
  if (TEST_SRC)
    add_executable(test_preprocessing ${TEST_SRC})
    target_include_directories(test_preprocessing PRIVATE
      ${PROJECT_SOURCE_DIR}/include
      ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(test_preprocessing PRIVATE
      ${Boost_LIBRARIES}
      # sbpl
      # GTest::GTest
    )
    add_test(NAME preprocessing COMMAND test_preprocessing)
  else()
    message(WARNING "BUILD_TESTS=ON but no tests/*.cpp files found")
  endif()
endif()

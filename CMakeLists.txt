cmake_minimum_required(VERSION 3.10)
project(ctmp_mani LANGUAGES CXX)

# require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------
# 1) your main library / executable
# ----------------------------------------------------------------------------

# grab all your .cpps under src/
file(GLOB_RECURSE CTMP_SRC CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(ctmp_mani
  ${CTMP_SRC}
)

# make sure your headers are found
target_include_directories(ctmp_mani PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

# ----------------------------------------------------------------------------
# 2) optional: system nlohmann_json (otherwise you already have bundled
#    json.hpp in include/)
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
# 3) optional: SBPL or other libs
# ----------------------------------------------------------------------------
# find_package(SBPL REQUIRED)            # if you need SBPL
# target_link_libraries(ctmp_mani PRIVATE sbpl)  # etc…

# ----------------------------------------------------------------------------
# 4) tests
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
    )
    # link any test-only libraries here, e.g. sbpl, gtest, etc.
    # target_link_libraries(test_preprocessing PRIVATE sbpl GTest::GTest)

    add_test(NAME preprocessing COMMAND test_preprocessing)
  else()
    message(WARNING "BUILD_TESTS=ON but no tests/*.cpp files found")
  endif()
endif()

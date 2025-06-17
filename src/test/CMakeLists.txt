#
# Lines inspired by https://google.github.io/googletest/quickstart-cmake.html
#

# Note: include(GoogleTest)and enable_testing() already done in top CMakeLists.txt

add_executable(unitTests
  unitTests.cpp
)
target_link_libraries(unitTests lib_core GTest::gtest_main sfml-graphics sfml-audio box2d tmxlite)

# The next two lines enable CMake’s test runner to discover the tests included in the binary,
# using the GoogleTest CMake module.
include(GoogleTest)
gtest_discover_tests(unitTests)

# Recursively glob all files in the resources directory and its subdirectories
file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/resources/*)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.stamp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/resources.stamp
    DEPENDS ${RESOURCE_FILES}
)

# Custom target that depends on the stamp file
add_custom_target(copy-resources-test ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources.stamp)

# Ensure mainLauncher depends on copy-resources
add_dependencies(unitTests copy-resources-test)


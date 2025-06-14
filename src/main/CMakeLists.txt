# Create the executable
add_executable(mainLauncher
  mainLauncher.cpp
)

# Link it to the lib_core library
target_link_libraries(mainLauncher PUBLIC lib_core tmxlite sfml-graphics sfml-audio box2d)

# Ensure tmxlite include directories are available
target_include_directories(mainLauncher PRIVATE ${tmxlite_SOURCE_DIR}/tmxlite/include)

# Recursively glob all files in the resources directory and its subdirectories
file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/resources/*)

# Custom command to copy the entire resources directory and create a stamp file
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.stamp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/resources.stamp
    DEPENDS ${RESOURCE_FILES}
)

# Custom target that depends on the stamp file
add_custom_target(copy-resources ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources.stamp)

# Ensure mainLauncher depends on copy-resources
add_dependencies(mainLauncher copy-resources)
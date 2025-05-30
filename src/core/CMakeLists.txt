add_library(lib_core
  MyMain.cpp
  MyMain.h
  Hitbox.cpp
  Hitbox.h
 "WorldManager.h" "Entity.h" "Entity.cpp" "WorldManager.cpp")
target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics box2d)

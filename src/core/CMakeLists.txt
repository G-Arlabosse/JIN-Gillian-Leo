add_library(lib_core
  MyMain.cpp
  MyMain.h
  "Hurtbox.cpp"
  Hurtbox.h
 "WorldManager.h" "Entity.h" "Entity.cpp" "WorldManager.cpp" "Wall.h" "Wall.cpp" "MyMainTest.h" "MyMainTest.cpp" "Hitbox.h" "Hitbox.cpp")
target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics box2d)

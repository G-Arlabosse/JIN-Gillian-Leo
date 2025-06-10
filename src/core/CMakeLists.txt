add_library(lib_core
  "Constants.h"
  "MyMain.cpp" "MyMain.h"
  "Hurtbox.cpp" "Hurtbox.h"
  "WorldManager.cpp" "WorldManager.h"
  "Entity.cpp" "Entity.h"
  "Wall.cpp" "Wall.h"
  "MyMainTest.cpp" "MyMainTest.h"
  "Hitbox.cpp" "Hitbox.h" 
  "Player.cpp" "Player.h"
  "Enemy.cpp" "Enemy.h" 
  "LevelManager.cpp" "LevelManager.h" 
  "Graph.cpp" "Graph.h"
  "TextureHandler.cpp" "TextureHandler.h" 
  "LevelMediator.h" "LevelTransition.cpp" "LevelTransition.h" )
target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics box2d)

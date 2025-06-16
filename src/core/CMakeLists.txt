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
 "AnimationManager.cpp" "AnimationManager.h" 
 "LevelMediator.h" 
 "LevelTransition.cpp" "LevelTransition.h"  
 "WorldNotifier.h"
 "Health.cpp" "Health.h"
 "TextureManager.h" "TextureManager.cpp" 
 "MusicManager.h" "MusicManager.cpp"
 "SFMLOrthogonalLayer.hpp")
target_include_directories(lib_core PUBLIC ./)
target_link_libraries(lib_core PUBLIC sfml-graphics sfml-audio box2d tmxlite)

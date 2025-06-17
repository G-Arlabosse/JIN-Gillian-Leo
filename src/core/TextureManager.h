#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "Constants.h"

struct textureInfo {
  std::string path;
  std::vector<int> anim_columns;
};

const std::map<enum textureName, textureInfo> textureInfos = {
    {HEALTH_BAR, {"health_bar.png", {0}}},
     {STRAWBERRY, {"fraise_animated.png", {8}}},
      {CARROT, {"carrot.png", {6,6,6}}},
       {CORN, {"corn.png", {6,6,6}}}, 
  {ROOM_TRANSITION, {"room_block.png", {0}}},
    {SLASH, {"slash_v3.png", {9}}}};

const std::string textureCommonPath = "resources/textures/";

class TextureManager {
public:
  TextureManager();
  void loadTexture(enum textureName textureName);
  sf::Texture* getTexture(enum textureName textureName);
  std::vector<int> getAnimationColumns(enum textureName textureName);

private:
  std::vector<std::unique_ptr<sf::Texture>> textures;
  std::vector<std::vector<int>> texture_anim_columns;
};

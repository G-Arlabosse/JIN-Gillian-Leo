#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "Constants.h"

const std::map<enum textureName, std::string> texturePath = {
  {HEALTH_BAR, "health_bar.png"},
  {STRAWBERRY, "fraise_animated.png"},
  {CARROT, "carrot.png"},
  {CORN, "corn.png"}
};

const std::string textureCommonPath = "resources/textures/";

class TextureManager {
public:
  TextureManager();
  void loadTexture(enum textureName textureName);
  sf::Texture* getTexture(enum textureName textureName);

private:
  std::vector<std::unique_ptr<sf::Texture>> textures;
};

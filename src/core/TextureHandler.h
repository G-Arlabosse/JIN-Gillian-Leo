#pragma once

#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>

class TextureHandler {
 private:
  sf::Texture texture;
  std::unique_ptr<sf::Sprite> sprite;
  int lines;
  int columns;
  int size;
  float scale;
  sf::Vector2i frame_pos;
 public:
  TextureHandler(const sf::Texture& t, int lines, int columns);
  void changeAnimation(std::string_view name);
  void nextFrame();
  sf::Sprite getSprite() const;
  int getSize() const;
  float getScale() const;
  void setScale(float s);
};
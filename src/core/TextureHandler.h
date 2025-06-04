#pragma once

#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>

class TextureHandler {
 private:
  sf::Texture texture;
  std::unique_ptr<sf::Sprite> sprite;
  int lines;
  std::vector<int> animation_columns;
  int size_x;
  int size_y;
  float scale;
  sf::Vector2i frame_pos;
  std::vector<float> animation_speeds;
  int current_animation;
  long last_frame_time;
 public:
  TextureHandler(const sf::Texture& t, std::vector<int>, int tempo);
  void changeAnimation(int animation_number);
  void nextFrame();
  sf::Sprite getSprite() const;
  int getSize_x() const;
  int getSize_y() const;
  float getScale() const;
  void setScale(float s);
  void update(long clock);
};
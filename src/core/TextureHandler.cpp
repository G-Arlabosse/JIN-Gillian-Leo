#include "TextureHandler.h"
#include <algorithm>


TextureHandler::TextureHandler(const sf::Texture& t, std::vector<int> animation_columns, int tempo)
    : texture{t},
      lines{(int)animation_columns.size()},
      animation_columns{animation_columns},
      size{(int)t.getSize().x / *std::max_element(animation_columns.begin(), animation_columns.end())},
      frame_pos{sf::Vector2i{0, 0}},
      current_animation{0} {
  sprite = std::make_unique<sf::Sprite>(t);
  animation_speeds = std::vector<float>(animation_columns.size());
  animation_columns.assign(animation_columns.begin(), animation_columns.end());
  for (int i = 0; i < animation_columns.size(); i++) {
    animation_speeds[i] = (float)tempo / (float)animation_columns[i];
  }
}

void TextureHandler::changeAnimation(int animation_number) {
  frame_pos = {0, size*animation_number};
  current_animation = animation_number;
}

void TextureHandler::nextFrame() {
  frame_pos.x = (frame_pos.x + size) % (size * animation_columns[current_animation]);
  sprite = std::make_unique<sf::Sprite>(texture, sf::IntRect(frame_pos, {size, size}));
}

sf::Sprite TextureHandler::getSprite() const { return *sprite; }

int TextureHandler::getSize() const { return size; }

float TextureHandler::getScale() const { return scale; }

void TextureHandler::setScale(float s) { scale = s; }

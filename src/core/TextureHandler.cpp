#include "TextureHandler.h"
#include <algorithm>

//Passer un vector<int> en argument du constructeur ?
TextureHandler::TextureHandler(const sf::Texture& t, std::vector<int> animation_columns, int tempo)
    : texture{t},
      lines{(int)animation_columns.size()},
      animation_columns{animation_columns},
      size_x{(int)t.getSize().x / *std::max_element(animation_columns.begin(), animation_columns.end())},
      size_y{(int)t.getSize().y / (int)animation_columns.size()},
      frame_pos{sf::Vector2i{0, 0}},
      current_animation{0} 
{
  sprite = std::make_unique<sf::Sprite>(t);
  animation_speeds = std::vector<float>(lines);
  animation_columns.assign(animation_columns.begin(), animation_columns.end());
  for (int i = 0; i < lines; i++) {
    animation_speeds[i] = (float)tempo / (float)animation_columns[i];
  }
  last_frame_time = std::clock();
}

void TextureHandler::changeAnimation(int animation_number) {
  frame_pos = {0, size_y*animation_number};
  current_animation = animation_number;
}

void TextureHandler::nextFrame() {
  frame_pos.x = (frame_pos.x + size_x) % (size_x * animation_columns[current_animation]);
  sprite->setTextureRect(sf::IntRect(frame_pos, { size_x, size_y }));
}

sf::Sprite TextureHandler::getSprite() const { return *sprite; }

int TextureHandler::getSize_x() const { return size_x; }

int TextureHandler::getSize_y() const { return size_y; }

float TextureHandler::getScale() const { return scale; }

void TextureHandler::setScale(float s) { scale = s; }

void TextureHandler::update(long clock) {
  if ((double)clock >
      (double)last_frame_time + (double)animation_speeds[current_animation]) {
    last_frame_time = clock;
    nextFrame();
  }
}

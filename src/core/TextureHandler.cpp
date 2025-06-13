#include "TextureHandler.h"
#include <algorithm>

//Passer un vector<int> en argument du constructeur ?
TextureHandler::TextureHandler(enum textureName textureName, TextureManager* textureManager,
  std::vector<int> animation_columns, float scale,
  int tempo, float speed_mult) :
  texture{ textureManager->getTexture(textureName) },
  lines{ (int)animation_columns.size() },
  animation_columns{ animation_columns },
  frame_pos{ sf::Vector2i{0, 0} },
  speed_mult{ speed_mult },
  current_animation{ 0 }
{
  size_x = (int)texture->getSize().x / *std::max_element(animation_columns.begin(), animation_columns.end());
  size_y = (int)texture->getSize().y / (int)animation_columns.size();

  sprite = std::make_unique<sf::Sprite>(*texture);
  sprite->setTextureRect(sf::IntRect(frame_pos, {size_x, size_y}));
  setScale(scale);
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

void TextureHandler::setScale(float s) { 
  scale = s;
  sprite->setScale({s, s});
}

void TextureHandler::update(long clock) {
  if ((double)clock >
      (double)last_frame_time + (double)animation_speeds[current_animation]/speed_mult) {
    last_frame_time = clock;
    nextFrame();
  }
}

void TextureHandler::draw(sf::RenderWindow* window, float pos_x, float pos_y) {
  sprite->setOrigin({(float)size_x / 2.f, (float)size_y / 2.f});
  sprite->setPosition(sf::Vector2f(pos_x, pos_y));

  window->draw(*sprite);
}
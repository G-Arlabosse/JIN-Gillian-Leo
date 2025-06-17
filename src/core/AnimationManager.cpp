#include "AnimationManager.h"
#include <algorithm>
#include <iostream>

//Passer un vector<int> en argument du constructeur ?
AnimationManager::AnimationManager(enum textureName textureName, TextureManager* textureManager, float scale,
  int tempo, float speed_mult, bool animation_loops) :
  frame_pos{ sf::Vector2i{0, 0} },
  speed_mult{ speed_mult },
  current_animation{ 0 },
  animation_loops{animation_loops},
  can_play{animation_loops},
      frames_played{0} {
  auto texture = textureManager->getTexture(textureName);
  animation_columns = textureManager->getAnimationColumns(textureName);
  lines = (int)animation_columns.size();

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

void AnimationManager::changeAnimation(int animation_number) {
  frame_pos = {0, size_y*animation_number};
  current_animation = animation_number;
}

void AnimationManager::nextFrame() {
  if (can_play) {
    frame_pos.x += size_x;
    frames_played++;
    if (frames_played == animation_columns[current_animation]) {
      if (!animation_loops) 
        can_play = false;
      frames_played = 0;
      frame_pos.x = 0;
    }     
    sprite->setTextureRect(sf::IntRect(frame_pos, {size_x, size_y}));
  }
}

sf::Sprite AnimationManager::getSprite() const { return *sprite; }

int AnimationManager::getSize_x() const { return size_x; }

int AnimationManager::getSize_y() const { return size_y; }

float AnimationManager::getScale() const { return scale; }

void AnimationManager::setRotation(float angle) {
  sprite->setRotation(sf::radians(angle));
}

void AnimationManager::setScale(float s) { 
  scale = s;
  sprite->setScale({s, s});
}

void AnimationManager::update(long clock) {
  if ((double)clock >
      (double)last_frame_time + (double)animation_speeds[current_animation]/speed_mult) {
    last_frame_time = clock;
    nextFrame();
  }
}

void AnimationManager::draw(sf::RenderWindow* window, float pos_x, float pos_y) {
  sprite->setOrigin({(float)size_x / 2.f, (float)size_y / 2.f});
  sprite->setPosition(sf::Vector2f(pos_x, pos_y));

  window->draw(*sprite);
}

void AnimationManager::reactivate() { 
  can_play = true;
  frames_played = 0;
}
#include "TextureHandler.h"


TextureHandler::TextureHandler(const sf::Texture& t, int lines, int columns)
    : texture{t},
      lines{lines},
      columns{columns},
      size{(int)t.getSize().x / columns},
      frame_pos{sf::Vector2i{0, 0}} {
  sprite = std::make_unique<sf::Sprite>(t);
}

void TextureHandler::changeAnimation(std::string_view name) {
  if (name == "Idle") {
    frame_pos = {0, 0};
  } else if (name == "Attack") {
    frame_pos = {0, size};  
  }
}

void TextureHandler::nextFrame() {
  frame_pos.x = (frame_pos.x + size) % (size * columns);
  sprite = std::make_unique<sf::Sprite>(texture, sf::IntRect(frame_pos, {size, size}));
}

sf::Sprite TextureHandler::getSprite() const { return *sprite; }

int TextureHandler::getSize() const { return size; }

float TextureHandler::getScale() const { return scale; }

void TextureHandler::setScale(float s) { scale = s; }

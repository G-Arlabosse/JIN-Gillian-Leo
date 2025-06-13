#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager() {
  for (int i = 0; i < textureName::NB_TEXTURES; i++) {
    textures.push_back(nullptr);
  }
}

void TextureManager::loadTexture(enum textureName textureName) {
  sf::Texture texture;
  auto path = commonPath + texturePath.at(textureName);
  if (!texture.loadFromFile(path)) {
    std::cerr << "Echec du chargement de la texture de test\n";
    terminate();
  }
  textures[textureName] = std::make_unique<sf::Texture>(texture);
}

sf::Texture* TextureManager::getTexture(enum textureName textureName) {
  if (textures[textureName] == nullptr) {
    loadTexture(textureName);
  }
  return textures[textureName].get();
}
#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager() {
  for (int i = 0; i < textureName::NB_TEXTURES; i++) {
    textures.push_back(nullptr);
    texture_anim_columns.emplace_back(std::vector<int>{});
  }
}

void TextureManager::loadTexture(enum textureName textureName) {
  sf::Texture texture;
  auto path = textureCommonPath + textureInfos.at(textureName).path;
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

std::vector<int> TextureManager::getAnimationColumns(enum textureName textureName) {
  if (texture_anim_columns[textureName] == std::vector<int>{}) {
    texture_anim_columns[textureName] =
        textureInfos.at(textureName).anim_columns;
  }
  return texture_anim_columns[textureName];
}
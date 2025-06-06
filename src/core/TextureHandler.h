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
	int size;
	float scale;
	sf::Vector2i frame_pos;
	std::vector<float> animation_speeds;
	int current_animation;
	long last_frame_time;
 public:
	//Constructor
	TextureHandler(const sf::Texture& t, std::vector<int>, int tempo);
  
	//Unused for now, will change the animation type later on
	void changeAnimation(int animation_number);
	
	//Changes the frame of the animation
	void nextFrame();

	//Updates the animation
	void update(long clock);

	//GETTER: returns the texture's size
	int getSize() const;
	
	//GETTER: returns the texture's size
	float getScale() const;

	//GETTER: returns the sprite adapted to the texture
	sf::Sprite getSprite() const;
  
	//SETTER: sets the scale of the sprite
	void setScale(float s);
	//TODO-Replace with a render(sf::RenderWindow *window) method ?
};
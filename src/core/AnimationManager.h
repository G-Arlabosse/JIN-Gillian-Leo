#pragma once

#include "SFML/Graphics.hpp"
#include "TextureManager.h"
#include "Constants.h"
#include <vector>
#include <memory>

class AnimationManager {
 private:
	TextureManager* textureManager;
  std::unique_ptr<sf::Sprite> sprite;
  int lines;
  std::vector<int> animation_columns;
  int size_x;
  int size_y;
  float scale;
  sf::Vector2i frame_pos;
  std::vector<float> animation_speeds;
  float speed_mult;
  int current_animation;
  long last_frame_time;

  bool animation_loops;
  bool can_play;
  int frames_played;
 public:
	//Constructor
	AnimationManager(enum textureName textureName, TextureManager* textureManager, float scale, int tempo, float speed_mult, bool animation_loops);
  
	//Unused for now, will change the animation type later on
	void changeAnimation(int animation_number);
	
	//Changes the frame of the animation
	void nextFrame();

	//Updates the animation
	void update(long clock);

	//GETTER: returns the texture's width
  int getSize_x() const;

	//GETTER: returns the texture's height
  int getSize_y() const;

	// GETTER: returns the texture's sprite position
	sf::Vector2i getPosition() const;
	
	//GETTER: returns the texture's scale
	float getScale() const;

	//GETTER: returns the sprite adapted to the texture
	sf::Sprite getSprite() const;
  
	void setRotation(float angle);

	//SETTER: sets the scale of the sprite
	void setScale(float s);
	//TODO-Replace with a render(sf::RenderWindow *window) method ?

	// Draws the current state of the texture centered at (pos_x, pos_y)
	void draw(sf::RenderWindow* window, float pos_x, float pos_y);

	// Allows a non-looping animation to be played again
	void reactivate();
};
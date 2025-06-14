#pragma once

#include "SFML/Audio.hpp"
#include <map>
#include <string>
#include "Constants.h"

//MusicStruct
const struct musicStruct {
  std::string path;
  int bpm;
};

//All music definitions

const struct musicStruct discoDescent { "disco_descent.mp3", 115 };


//Map between musicName and the structs
const std::map<enum musicName, struct musicStruct> musicMap = {
  {musicName::DISCO_DESCENT, discoDescent},
};

const std::string musicCommonPath = "resources/music/";

class MusicManager {
public:
  MusicManager();
  ~MusicManager() = default;
  void playMusic(enum musicName name);
  int getMusicBPM(enum musicName name);
  void stopMusic();
private:
  sf::Music music;
};

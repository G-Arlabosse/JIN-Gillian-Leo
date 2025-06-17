#include "MusicManager.h"

MusicManager::MusicManager() {}

void MusicManager::playMusic(enum musicName name) {
  auto path = musicCommonPath + musicMap.at(name).path;
  if (!music.openFromFile(path)) {
    terminate();
  }
  music.play();
}

int MusicManager::getMusicBPM(enum musicName name) {
  return musicMap.at(name).bpm;
}

void MusicManager::stopMusic() {
  music.stop();
}
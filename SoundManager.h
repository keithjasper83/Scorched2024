#pragma once
#include <SFML/Audio.hpp>
#include "FileSystem.h"
#include <iostream>

class SoundManager
{
  public:
    SoundManager();
    ~SoundManager();
    void Fire();
    void Explode();

  private:
    sf::SoundBuffer SoundBuffer_FireProjectile;
    sf::SoundBuffer SoundBuffer_Explosion;
    sf::Sound Sound_FireProjectile;
    sf::Sound Sound_Explosion;

    void LoadSounds();
    void LoadSound(std::string soundName, sf::SoundBuffer &soundBuffer, sf::Sound &sound);
};

#pragma once
#include "FileSystem.h"
#include <SFML/Audio.hpp>
#include <iostream>

class sound_manager
{
  public:
    sound_manager();
    void fire();
    void explode();

  private:
    sf::SoundBuffer sound_buffer_fire_projectile_;
    sf::SoundBuffer sound_buffer_explosion_;
    sf::Sound sound_fire_projectile_;
    sf::Sound sound_explosion_;

    void load_sounds();
    void load_sound(std::string sound_name, sf::SoundBuffer &sound_buffer, sf::Sound &sound);
};

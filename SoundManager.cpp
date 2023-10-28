#include <iostream>
#include <SFML/Audio.hpp>

// Constructor
class SoundManager {
public:
    SoundManager() {
        if (!sb_fire.loadFromFile("9mm-pistol-shoot-short-reverb-7152.wav")) {
            std::cout << "ERROR: 9mm-pistol-shoot-short-reverb-7152.wav" << std::endl;
        }
        else {
            s_fire.setBuffer(sb_fire);
        }
    }

    //Functions
    void Fire() {
        printf("FIRE!\n");
        s_fire.play();
    }


private:
    sf::SoundBuffer sb_fire;
    sf::Sound s_fire;
    sf::SoundBuffer sb_astHit;
    sf::Sound s_astHit;
};
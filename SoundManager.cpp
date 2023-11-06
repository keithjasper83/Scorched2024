#pragma once

#include "SoundManager.h" //#

// todo: implement simple function to call anywhere which will play buffers with a single line of code

/// <summary>
/// <para>SoundManager Class</para>
/// <para>Loads and plays sounds</para>
/// <para>SoundManager::Fire() - Plays the Fire sound</para>
/// <para>SoundManager::Explode() - Plays the Explosion sound</para>
/// </summary>

/// <summary>
/// Constructor
/// </summary>
SoundManager::SoundManager()
{
    LoadSounds();
}

/// <summary>
/// Destructor
/// </summary>
SoundManager::~SoundManager()
{
}

/// <summary>
/// <para>Fire() - Plays the Fire sound</para>
/// </summary>
/// <example>
/// <code>
/// SoundManager soundManager;
/// soundManager.Fire();
/// </code>
/// </example>
/// <returns>void</returns>
/// <remarks>none</remarks>
/// <see cref="SoundManager"/>
void SoundManager::Fire()
{
    std::cout << "SoundManager::Fire()" << std::endl;
    Sound_FireProjectile.play();
}

/// <summary>
/// <para>Explode() - Plays the Explosion sound</para>
/// </summary>
/// <example>
/// <code>
/// SoundManager soundManager;
/// soundManager.Explode();
/// </code>
/// </example>
/// <returns>void</returns>
/// <remarks>none</remarks>
/// <see cref="SoundManager"/>
void SoundManager::Explode()
{
    std::cout << "SoundManager::Explode()" << std::endl;
    Sound_Explosion.play();
}

/// <summary>
/// <para>LoadSounds() - Loads the sounds</para>
/// </summary>
/// <example>
/// <code>
/// SoundManager soundManager;
/// soundManager.LoadSounds();
/// </code>
/// </example>
/// <returns>void</returns>
/// <remarks>none</remarks>
/// <see cref="SoundManager"/>
void SoundManager::LoadSounds()
{
    SoundManager::LoadSound("mixkit-blast-hit-with-echo-2186.wav", SoundBuffer_Explosion, Sound_Explosion);
    SoundManager::LoadSound("9mm-pistol-shoot-short-reverb-7152.wav", SoundBuffer_FireProjectile, Sound_FireProjectile);
}

/// <summary>
/// <para>Load_FireProjectile() - Loads the FireProjectile sound</para>
/// </summary>
/// <returns>void</returns>
void SoundManager::LoadSound(std::string soundName, sf::SoundBuffer &soundBuffer, sf::Sound &sound)
{
    std::string Filename = "Sounds/" + soundName;
    if (FileSystem::fileExists(Filename))
    {
        if (!soundBuffer.loadFromFile(Filename))
        {
            std::cout << "ERROR File not found - " << Filename << std::endl;
        }
        else
        {
            sound.setBuffer(soundBuffer);
        }
    }
    else
    {
        std::cout << "ERROR: File does not exist: " << Filename << std::endl;
    }
}

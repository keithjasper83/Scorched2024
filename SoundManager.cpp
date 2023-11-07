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
sound_manager::sound_manager()
{
    load_sounds();
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
void sound_manager::fire()
{
    std::cout << "sound_manager::fire()" << std::endl;
    sound_fire_projectile_.play();
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
void sound_manager::explode()
{
    std::cout << "sound_manager::explode()" << std::endl;
    sound_explosion_.play();
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
void sound_manager::load_sounds()
{
    sound_manager::load_sound("mixkit-blast-hit-with-echo-2186.wav", sound_buffer_explosion_, sound_explosion_);
    sound_manager::load_sound("9mm-pistol-shoot-short-reverb-7152.wav", sound_buffer_fire_projectile_,
                              sound_fire_projectile_);
}

/// <summary>
/// <para>Load_FireProjectile() - Loads the FireProjectile sound</para>
/// </summary>
/// <returns>void</returns>
void sound_manager::load_sound(std::string sound_name, sf::SoundBuffer &sound_buffer, sf::Sound &sound)
{
    std::string Filename = "Sounds/" + sound_name;
    if (file_system::file_exists(Filename))
    {
        if (!sound_buffer.loadFromFile(Filename))
        {
            std::cout << "ERROR File not found - " << Filename << std::endl;
        }
        else
        {
            sound.setBuffer(sound_buffer);
        }
    }
    else
    {
        std::cout << "ERROR: File does not exist: " << Filename << std::endl;
    }
}

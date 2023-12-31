#include "SoundManager.h"

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
    KJ::debug_output::print(__FILE__, "sound_manager::fire()", KJ::debug_output::MessageType::GOOD);
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
    KJ::debug_output::print(__FILE__, "sound_manager::explode()", KJ::debug_output::MessageType::GOOD);
    sound_explosion_.play();
}

void sound_manager::rotate()
{
    KJ::debug_output::print(__FILE__, "sound_manager::cogs()", KJ::debug_output::MessageType::INFO);
    if (sound_cogs_.getStatus() != sf::Sound::Playing)
    {
        sound_cogs_.play();
    }
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
    sound_manager::load_sound("cogs.wav", sound_buffer_cogs_, sound_cogs_);
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
            KJ::debug_output::print(__FILE__, "File not found - " + Filename, KJ::debug_output::MessageType::FATAL);
        }
        else
        {
            sound.setBuffer(sound_buffer);
            sound.setLoop(false);
        }
    }
    else
    {
        KJ::debug_output::print(__FILE__, " File does not exist: " + Filename, KJ::debug_output::MessageType::FATAL);
    }
}

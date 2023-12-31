# SFMLScorched
A simple Scorched Earth clone written in C++ using SFML.
Every publish of this project will be a "working" version, it may be buggy but will always work when compiled and ran with the latest version of Visual Studio.

## ChangeLog
19th November 2023

    - Added a menu system

13th November 2023

     - Added animation class and implemented for background particles


12th November 2023

 	- Implemented atmospheric particles (think snow or dust or rain), currently only snow (ish)
    - Fixed projectile launch origin to be the end of the turret barrel
    - Altered default turret angle for Players
    - Fixed some const declarations on functions and variables - more to follow


## ToDo
 - Update Menu system to have a settings page (accepting inputs/toggles etc)
 - Add player names (and ability to set)
 - Add player colours (and ability to set)
 
 - Fix animation framerates, they are currently not consistent
 - Add a game over screen
 - Add a game win screen
 - Add player rotation
 - Add player scores - and implemnt a scoring system
 - Add a wind system
 - Add a wind direction indicator
 - Add a wind speed indicator
 - Add additional weapons
 - Add a weapon selection system
 - Add a weapon selection indicator
 - After all of the above add a weapon purchase system
 - Add a weapon purchase menu
 - Create a system to allow players to earn currency
 - Add a currency indicator
 - Update Tank Textures
 - Add a tank selection system
 - Update Terrain Textures
 - Add a terrain selection system
 - Implement texturing on the HUD
 - Implement additional explosions
 - Implement additional atmospheric particles
 - Implement additional sound effects
 - Implement Music
 - Implement a sound volume control
 - Implement a music volume control
 - Implement a sound mute control
 - Implement a music mute control
 - Implement a sound mute all control

## A work in progress
Due to the nature of this project, it is a work in progress. I will be adding features as I go along. Take note of the changelog for functionality that get added

## Support
This project is currently only supported on Windows, however, I will be adding support for Linux and Mac OS X in the future.

## Dependencies
This project requires the following dependencies:
* SFML 2.4.2

## Contributing
If you wish to contribute to this project, please fork this repository and create a pull request with your changes.
However, please check out my other projects as these are likley to be more active than this.

-- Thanks, @83keith
-- X: @podman99
-- Email: keith_83@icloud.com :Use subject "GitHub: SFMLScorched"

## InGame Keyboard Shortcuts
* `F5` - Toggle Fullscreen
* `Q` - Quit Game
* `P` - Play Test sound
* `F1` - Reset Game
* `G` - Toggle Pixel Grid
* `Left` - Change Turret Angle
* `Right` - Change Turret Angle
* `Up` - Change Turret Power
* `Down` - Change Turret Power
* `Space` - Fire
* `F` - Toggle FPS
* `O` - Disable Falling Physics
* `I` - Enable Atmospheric Particles
* `Mouse Click` - Remove Terrain at Location
* `Mouse Drag` - Remove Terrain in Area
* `ESC` - Display Menu
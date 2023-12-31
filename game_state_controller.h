#pragma once
#include "debug_output.h"
class game_state_controller
{
  public:
    game_state_controller() = default;
    ~game_state_controller() = default;
    /// <summary>
    /// Gets the player count
    /// </summary>
    /// <returns>int number of players</returns>
    int get_players() const;
    /// <summary>
    /// Gets the current players tern
    /// </summary>
    /// <returns>int current players turn</returns>
    int get_turn() const;
    /// <summary>
    /// sets the total number of players
    /// </summary>
    /// <param name="players">int set quantity of players</param>
    /// <returns>int qty of players</returns>
    int set_players(int players);
    /// <summary>
    /// Rotate the current players turn
    /// </summary>
    /// <returns>int new turn</returns>
    int rotate_turn();

  private:
    int players = 0;
    int turn = 0;
};

#include "game_state_controller.h"

int game_state_controller::get_players() const
{
    return this->players;
}

int game_state_controller::get_turn() const
{
    return this->turn;
}

int game_state_controller::set_players(int players)
{
    this->players = players;
    return this->players;
}

int game_state_controller::rotate_turn()
{
    if (this->turn <= this->players)
    {
        this->turn++;
    }
    else
    {
        this->turn = 1;
    }
    return this->turn;
}

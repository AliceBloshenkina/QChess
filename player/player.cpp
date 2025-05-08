#include "player.h"

void Player::set_name(const std::string& name_)
{
    this->name = name_;
}
void Player::set_rating(int rating_)
{
    this->rating = rating_;
}
int Player::get_rating() const
{
    return this->rating;
}
std::string Player::get_name() const
{
    return name;
}

Color Player::get_color() const
{
    return color;
}

double Player::get_time() const
{
    return time;
}

int Player::get_kills_pieces() const
{
    return kills_piece;
}

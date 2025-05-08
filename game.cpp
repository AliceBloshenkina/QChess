#include "game.h"

Matrix<ChessPiece*> Game::get_board()
{
    return board.getBoard();
}
Board& Game::get_b()
{
    return board;
}
Color Game::get_current_color() const
{
    return currentPlayer;
}
void Game::set_current_color()
{
    currentPlayer = Color_opposite::opposite(currentPlayer);
}

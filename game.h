#ifndef GAME_H
#define GAME_H

#include "board/board.h"
#include "player/player.h"


class Game {
public:
    Game(const std::string& name1, int rating1, const std::string& name2, int rating2)
        : player1(Color::WHITE, name1, rating1), player2(Color::BLACK, name2, rating2), currentPlayer(Color::WHITE)
    {
        board.fromFEN("C:\\Users\\User\\Documents\\untitled1\\default_board.txt");
    }

    ~Game() = default;

    void MakeMove();

    void RunGame();

    void EndGame();

    [[nodiscard]] bool IsGameOver() const;

    [[nodiscard]] bool IsCheck() const;
    Color get_current_color() const;
    void set_current_color();
    void CheckGameStatus();
    Board& get_b();
    Matrix<ChessPiece*> get_board();
private:
    Board board;
    Player player1;
    Player player2;
    Color currentPlayer;
};
#endif

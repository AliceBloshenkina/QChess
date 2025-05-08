#ifndef BOARD_H
#define BOARD_H

#include "../pieces/pieces.h"
#include <windows.h>
#include "../matrix/matrix.h"
#include <mutex>
#include <thread>
#include <algorithm>

class Board {
public:

    Board();

    ~Board();

    [[nodiscard]] bool isEmptyChessPiece(Position pos, Color color) const;


    void set_piece(Position pos_, Position new_pos);

    bool is_valid_opponentKing_attack(Position ptr, Color col)const;


    [[nodiscard]] std::vector<Position> GetValidMoves(Position position) const;


    [[nodiscard]] std::vector<Position> GetValidAttacks(Position position) const;


    [[nodiscard]]  Matrix<ChessPiece *> &getBoard();


    [[nodiscard]] bool IsKingInCheck(Color color) const;


    [[nodiscard]] bool isValidMove(Position from, Position to) const;


    ChessPiece *GetPieceAt(Position pos);


    void fromFEN(const std::string &filename);

    [[nodiscard]] std::vector<ChessPiece *> get_opposite_color_pieces(Color color) const;

    bool is_attacked(Position &position, Color color) const;
    bool is_mate(Color color) const;
    bool is_pate(Color& color) const;
    Board& operator=(const Board& other);
    Position Current_king_pos(Color col);
    void del_from_vector(Position pos, Color col);
    ChessPiece *createChessPiece(char symbol, Color color, int row, int col);

private:


    Matrix<ChessPiece *> board;

    ChessPiece *white_king;

    ChessPiece *black_king;
    bool check_after_move(Color col, Position from, Position to) const;

    std::vector<ChessPiece *> whitePieces;

    std::vector<ChessPiece *> blackPieces;

    std::vector<std::string> gameHistory;
    void clearBoard();
};

#endif // BOARD_H

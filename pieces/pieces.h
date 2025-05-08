#ifndef PIECES_H
#define PIECES_H

#include "../board/BoardUtils.h"
#include "../other/Color_opposite.h"
#include <QCoreApplication>

class Pawn : public ChessPiece {
public:

    Pawn(Color color, int r, int c) : ChessPiece(color, r, c) {for_attack = false;}
    virtual ChessPiece* clone() const override
    {
        return new Pawn(*this);
    }

    std::vector<CoordinateShift *> ValidShift() override
    {
        std::vector<CoordinateShift *> ValidSh;
        if (color == Color::BLACK)
        {
            if(!for_attack)
            {
                ValidSh.push_back(new CoordinateShift(1, 0));
                if (!is_moving)
                {
                    ValidSh.push_back(new CoordinateShift(2, 0));
                }
            }
            ValidSh.push_back(new CoordinateShift(1, 1));
            ValidSh.push_back(new CoordinateShift(1, -1));
        } else {
            if(!for_attack)
            {
                ValidSh.push_back(new CoordinateShift(-1, 0));
                if (!is_moving)
                {
                    ValidSh.push_back(new CoordinateShift(-2, 0));
                }
            }
            ValidSh.push_back(new CoordinateShift(-1, -1));
            ValidSh.push_back(new CoordinateShift(-1, 1));
        }
        return ValidSh;
    }

    bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const override
    {
        if(for_attack)
        {
            return this->get_pos().getCol() != position.getCol();
        }
        return (board[position.getRow()][position.getCol()] == nullptr &&
                this->get_pos().getCol() == position.getCol() && BoardUtils::vert_hor_coordinate_between(this->get_pos(), position, board)) ||
               (board[position.getRow()][position.getCol()] != nullptr &&
                (board[position.getRow()][position.getCol()]->GetColor() != this->GetColor() &&
                 this->get_pos().getCol() != position.getCol()));
    }

    char get_name() override
    {
        return 'P';
    }
    QString get_file() override
    {
        if(color == Color::BLACK)
        {
            return QCoreApplication::applicationDirPath() + "/img/pawn";
        }
        return QCoreApplication::applicationDirPath() + "/img/pawn1";
    }
};

class Queen : public ChessPiece {
public:

    Queen(Color color, int r, int c) : ChessPiece(color, r, c) {for_attack = false;}
    virtual ChessPiece* clone() const override
    {
        return new Queen(*this);
    }

    std::vector<CoordinateShift *> ValidShift() override
    {
        std::vector<CoordinateShift *> ValidSh;
        for (int i = 1; i < 8; i++)
        {
            ValidSh.push_back(new CoordinateShift(i, i));
            ValidSh.push_back(new CoordinateShift(-i, i));
            ValidSh.push_back(new CoordinateShift(-i, -i));
            ValidSh.push_back(new CoordinateShift(i, -i));
            ValidSh.push_back(new CoordinateShift(i, 0));
            ValidSh.push_back(new CoordinateShift(0, i));
            ValidSh.push_back(new CoordinateShift(-i, 0));
            ValidSh.push_back(new CoordinateShift(0, -i));
        }
        return ValidSh;
    }

    char get_name() override
    {
        return 'Q';
    }
    QString get_file() override
    {
        if(color == Color::BLACK)
        {
            return QCoreApplication::applicationDirPath() + "/img/queen";
        }
        return QCoreApplication::applicationDirPath() + "/img/queen1";
    }

    bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const override
    {
        if(for_attack)
        {
            if (BoardUtils::diagonal_coordinate_between(this->get_pos(), position, board) &&
                BoardUtils::vert_hor_coordinate_between(this->get_pos(), position, board))
            {
                return true;
            }
            return false;
        }
        bool result = board[position.getRow()][position.getCol()] == nullptr ||
                      board[position.getRow()][position.getCol()]->GetColor() != this->GetColor();
        if (result && BoardUtils::diagonal_coordinate_between(this->get_pos(), position, board) &&
            BoardUtils::vert_hor_coordinate_between(this->get_pos(), position, board))
        {
            return true;
        }
        return false;
    }

};

class Rook : public ChessPiece {
public:

    Rook(Color color, int r, int c) : ChessPiece(color, r, c) {for_attack = false;}
    virtual ChessPiece* clone() const override
    {
        return new Rook(*this);
    }

    std::vector<CoordinateShift *> ValidShift() override
    {
        std::vector<CoordinateShift *> ValidSh;
        for (int i = 1; i < 8; i++)
        {
            ValidSh.push_back(new CoordinateShift(i, 0));
            ValidSh.push_back(new CoordinateShift(0, i));
            ValidSh.push_back(new CoordinateShift(-i, 0));
            ValidSh.push_back(new CoordinateShift(0, -i));

        }
        return ValidSh;
    }

    char get_name() override
    {
        return 'R';
    }
    QString get_file() override
    {
        if(color == Color::BLACK)
        {
            return QCoreApplication::applicationDirPath() + "/img/rook";
        }
        return QCoreApplication::applicationDirPath() + "/img/rook1";
    }

    bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const override
    {
        if(for_attack){
            if (BoardUtils::vert_hor_coordinate_between(this->get_pos(), position, board))
            {
                return true;
            }
            return false;
        }
        bool result = board[position.getRow()][position.getCol()] == nullptr ||
                      board[position.getRow()][position.getCol()]->GetColor() != this->GetColor();
        if (result && BoardUtils::vert_hor_coordinate_between(this->get_pos(), position, board))
        {
            return true;
        }
        return false;
    }
};

class Horse : public ChessPiece {
public:

    Horse(Color color, int r, int c) : ChessPiece(color, r, c) {for_attack = false;}
    virtual ChessPiece* clone() const override
    {
        return new Horse(*this);
    }

    std::vector<CoordinateShift *> ValidShift() override
    {
        std::vector<CoordinateShift *> ValidSh;

        ValidSh.push_back(new CoordinateShift(1, 2));
        ValidSh.push_back(new CoordinateShift(2, 1));

        ValidSh.push_back(new CoordinateShift(2, -1));
        ValidSh.push_back(new CoordinateShift(1, -2));

        ValidSh.push_back(new CoordinateShift(-2, -1));
        ValidSh.push_back(new CoordinateShift(-1, -2));

        ValidSh.push_back(new CoordinateShift(-2, 1));
        ValidSh.push_back(new CoordinateShift(-1, 2));

        return ValidSh;
    }

    bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const override
    {
        if(for_attack)
        {
            return true;
        }
        return board[position.getRow()][position.getCol()] == nullptr ||
               board[position.getRow()][position.getCol()]->GetColor() != this->GetColor();
    }

    char get_name() override
    {
        return 'H';
    }
    QString get_file() override
    {
        if(color == Color::BLACK)
        {
            return QCoreApplication::applicationDirPath() + "/img/horse";
        }
        return QCoreApplication::applicationDirPath() + "/img/horse1";
    }
};

class Bishop : public ChessPiece {

public:

    Bishop(Color color, int r, int c) : ChessPiece(color, r, c) {for_attack = false;}
    virtual ChessPiece* clone() const override
    {
        return new Bishop(*this);
    }

    std::vector<CoordinateShift *> ValidShift() override
    {
        std::vector<CoordinateShift *> ValidSh;

        for (int i = 1; i < 8; i++)
        {
            ValidSh.push_back(new CoordinateShift(i, i));
            ValidSh.push_back(new CoordinateShift(i, -i));
            ValidSh.push_back(new CoordinateShift(-i, i));
            ValidSh.push_back(new CoordinateShift(-i, -i));
        }
        return ValidSh;
    }

    char get_name() override
    {
        return 'B';
    }
    QString get_file() override
    {
        if(color == Color::BLACK)
        {
            return QCoreApplication::applicationDirPath() + "/img/bishop";
        }
        return QCoreApplication::applicationDirPath() + "/img/bishop1";
    }

    bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const override
    {
        if(for_attack)
        {
            if (BoardUtils::diagonal_coordinate_between(this->get_pos(), position, board))
                return true;
            return false;
        }
        bool result = board[position.getRow()][position.getCol()] == nullptr ||
                      board[position.getRow()][position.getCol()]->GetColor() != this->GetColor();
        if (result)
        {
            if (BoardUtils::diagonal_coordinate_between(this->get_pos(), position, board))
                return true;
        }
        return false;
    }
};

class King : public ChessPiece {

public:

    King(Color color, int r, int c) : ChessPiece(color, r, c) {}
    virtual ChessPiece* clone() const override
    {
        return new King(*this);
    }

    std::vector<CoordinateShift *> ValidShift() override
    {
        std::vector<CoordinateShift *> ValidSh;
        ValidSh.push_back(new CoordinateShift(1, 1));
        ValidSh.push_back(new CoordinateShift(-1, 1));
        ValidSh.push_back(new CoordinateShift(-1, -1));
        ValidSh.push_back(new CoordinateShift(1, -1));
        ValidSh.push_back(new CoordinateShift(1, 0));
        ValidSh.push_back(new CoordinateShift(0, 1));
        ValidSh.push_back(new CoordinateShift(-1, 0));
        ValidSh.push_back(new CoordinateShift(0, -1));
        if(!is_moving)
        {
            ValidSh.push_back(new CoordinateShift(0, 2));
            ValidSh.push_back(new CoordinateShift(0, -2));
        }
        return ValidSh;
    }

    char get_name() override
    {
        return 'K';
    }
    QString get_file() override
    {
        if(color == Color::BLACK)
        {
            return QCoreApplication::applicationDirPath() + "/img/king";
        }
        return QCoreApplication::applicationDirPath() + "/img/king1";
    }

    bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const override
    {
        bool result = board[position.getRow()][position.getCol()] == nullptr ||
                 board[position.getRow()][position.getCol()]->GetColor() != this->GetColor();
        if(pos.getCol() - 2 == position.getCol())
        {
            return board[pos.getRow()][0] && board[pos.getRow()][0]->GetColor() == color && board[pos.getRow()][0]->get_name() == 'R' && BoardUtils::vert_hor_coordinate_between(board[pos.getRow()][0]->get_pos(), pos, board);
        } else if(pos.getCol() + 2 == position.getCol()){

            return board[pos.getRow()][7] && board[pos.getRow()][7]->GetColor() == color && board[pos.getRow()][7]->get_name() == 'R' && BoardUtils::vert_hor_coordinate_between(board[pos.getRow()][7]->get_pos(), pos, board);
        } else {
            return result;
        }
    }

};
#endif

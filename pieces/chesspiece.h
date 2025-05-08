#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "../other/position.h"
#include "../matrix/matrix.h"
#include "../other/color.h"
#include <QString>

class ChessPiece {
public:

    ChessPiece(Color color, int r, int c) : color(color), pos(r, c), is_moving(false) {}
    virtual ChessPiece* clone() const = 0;
    ChessPiece& operator=(const ChessPiece& other)
    {
        if(this != &other)
        {
            color = other.color;
            pos = other.pos;
            is_moving = other.is_moving;
        }
        return *this;
    }

    virtual ~ChessPiece() = default;

    virtual std::vector<CoordinateShift *> ValidShift() = 0;

    [[nodiscard]] virtual bool isEmptyForMove(Position position, const Matrix<ChessPiece *> &board) const = 0;

    Color GetColor() const
    {
        return color;
    }

    virtual char get_name() = 0;

    virtual QString get_file() = 0;

    void setColor(Color color_)
    {
        color = color_;
    }

    void setPos(const Position &pos_)
    {
        pos = pos_;
    }

    Position get_pos() const
    {
        return this->pos;
    }

    bool isMoving() const
    {
        return is_moving;
    }

    void set_check(bool check)
    {
        for_attack = check;
    }

    void setIsMoving(bool isMoving) {
        is_moving = isMoving;
    }

    ChessPiece& operator=(const ChessPiece* other)
    {
        for_attack = other->for_attack;
        color = other->color;
        pos = other->pos;
        is_moving = other->is_moving;
        return *this;
    }

protected:
    bool for_attack;
    Color color;
    Position pos;
    bool is_moving;
};
#endif

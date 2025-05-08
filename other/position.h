#ifndef POSITION_H
#define POSITION_H
#include "CoordinateShift.h"

class Position {
public:

    Position() = default;

    Position(int r, int c) : row(r), col(c) {}

    int getRow() const
    {
        return row;
    }

    void setRow(int row_)
    {
        this->row = row_;
    }

    int getCol() const
    {
        return col;
    }

    void setCol(int col_)
    {
        this->col = col_;
    }

    bool canShift(CoordinateShift *shift) const
    {
        int r = row + shift->getRowshift();
        int c = col + shift->getColshift();
        return r >= 0 && r < 8 && c >= 0 && c < 8;
    }

    Position shift(CoordinateShift *shift) const
    {
        Position pos(row + shift->getRowshift(), col + shift->getColshift());
        return pos;
    }

    friend bool operator==(const Position &lhs, const Position &rhs)
    {
        return lhs.row == rhs.row && lhs.col == rhs.col;
    }

    friend bool operator!=(const Position &lhs, const Position &rhs)
    {
        return !(rhs == lhs);
    }

private:
    int row;
    int col;
};
#endif

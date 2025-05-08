#ifndef COORDINATESHIFT_H
#define COORDINATESHIFT_H

class CoordinateShift {
private:
    int rowshift{};
    int colshift{};
public:

    CoordinateShift(int rowshift_, int colshift_) : rowshift(rowshift_), colshift(colshift_) {}

    [[nodiscard]] int getRowshift() const
    {
        return rowshift;
    }

    [[nodiscard]] int getColshift() const
    {
        return colshift;
    }
};
#endif

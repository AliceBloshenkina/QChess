#ifndef UNTITLED174_BOARDUTILS_H
#define UNTITLED174_BOARDUTILS_H

#include "../other/position.h"
#include "../pieces/chesspiece.h"
#include "../matrix/matrix.h"

class BoardUtils{
public:

    static bool diagonal_coordinate_between(Position from, Position to, const Matrix<ChessPiece*>& board)
    {
        if((from.getRow() > to.getRow()) && (from.getCol() > to.getCol()))
        {
            for(int i = 1; i < from.getRow() - to.getRow(); i++)
            {
                if(board[from.getRow() - i][from.getCol() - i] != nullptr)
                {
                    return false;
                }
            }
        } else if (from.getRow() > to.getRow() &&from.getCol() < to.getCol()){
            for(int i = 1; i < from.getRow() - to.getRow(); i++)
            {
                if(board[from.getRow() - i][from.getCol() + i] != nullptr)
                {
                    return false;
                }
            }
        } else if(from.getRow() < to.getRow() && from.getCol() > to.getCol()){
            for(int i = 1; i < to.getRow() - from.getRow() ; i++)
            {
                if(board[from.getRow() + i][from.getCol() - i] != nullptr)
                {
                    return false;
                }
            }
        } else if(from.getRow() < to.getRow() && from.getCol() < to.getCol()){
            for(int i = 1; i < to.getRow() - from.getRow(); i++)
            {
                if(board[from.getRow() + i][from.getCol() + i] != nullptr)
                {
                    return false;
                }
            }
        }
        return true;
    }

    static bool vert_hor_coordinate_between(Position from, Position to, const Matrix<ChessPiece*>& board)
    {
        if(from.getRow() > to.getRow() && from.getCol() == to.getCol())
        {
            for(int i = 1; i < from.getRow() - to.getRow(); i++)
            {
                if(board[from.getRow() - i][from.getCol()] != nullptr)
                {
                    return false;
                }
            }
        } else if(from.getRow() < to.getRow() && from.getCol() == to.getCol()){
            for(int i = 1; i < to.getRow() - from.getRow(); i++)
            {
                if(board[from.getRow() + i][from.getCol()] != nullptr)
                {
                    return false;
                }
            }
        } else if(from.getRow() == to.getRow() && from.getCol() > to.getCol()){
            for(int i = 1; i < from.getCol() - to.getCol(); i++)
            {
                if(board[from.getRow()][from.getCol() - i] != nullptr)
                {
                    return false;
                }
            }
        } else  if(from.getRow() == to.getRow() && from.getCol() < to.getCol()){
            for(int i = 1; i < to.getCol() - from.getCol(); i++)
            {
                if(board[from.getRow()][from.getCol() + i] != nullptr)
                {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif

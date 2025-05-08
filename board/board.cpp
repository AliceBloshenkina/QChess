#include <fstream>
#include <sstream>
#include <QDebug>
#include "board.h"
#include <QString>

std::vector<Position> Board::GetValidMoves(Position position) const
{
    ChessPiece * ptr = board[position.getRow()][position.getCol()];
    std::vector<Position> th;
    std::vector<CoordinateShift*> ValidSh = ptr->ValidShift();
    for (const auto & i : ValidSh)
    {
        if (ptr->get_pos().canShift(i))
        {
            Position new_pos = ptr->get_pos().shift( i);
            if(board[ptr->get_pos().getRow()][ptr->get_pos().getCol()]->isEmptyForMove(new_pos,board))
            {
                if( ptr->get_name() != 'K')
                {
                    if(!check_after_move(ptr->GetColor(), ptr->get_pos(), new_pos))
                        th.push_back(new_pos);
                } else {
                    const_cast<Board*>(this)->board[position.getRow()][position.getCol()] = nullptr;
                    if (!is_attacked(new_pos, ptr->GetColor()) && !is_valid_opponentKing_attack(new_pos, ptr->GetColor())){
                        th.push_back(new_pos);
                    }
                    const_cast<Board*>(this)->board[position.getRow()][position.getCol()] = ptr;
                }
            }
        }
    }
    return th;
}

bool Board::is_valid_opponentKing_attack(Position ptr, Color col) const
{
    ChessPiece* opponentKing = (col == Color::WHITE) ? black_king:white_king;
    std::vector<CoordinateShift*> ValidSh = opponentKing->ValidShift();
    for (auto & i : ValidSh)
    {
        if (opponentKing->get_pos().canShift(i))
        {
            Position new_pos = opponentKing->get_pos().shift( i);
            if(board[opponentKing->get_pos().getRow()][opponentKing->get_pos().getCol()]->isEmptyForMove(new_pos,board))
            {
                if(new_pos == ptr)
                    return true;
            }
        }
    }
    return false;
}

Position Board::Current_king_pos(Color col)
{
    return (col == Color::WHITE) ? white_king->get_pos():black_king->get_pos();
}

std::vector<Position> Board::GetValidAttacks(Position position) const
{
    ChessPiece * ptr = board[position.getRow()][position.getCol()];
    std::vector<Position> th;
    ptr->set_check(true);
    std::vector<CoordinateShift*> ValidSh = ptr->ValidShift();
    for (auto & i : ValidSh)
    {
        if (ptr->get_pos().canShift(i))
        {
            Position new_pos = ptr->get_pos().shift( i);
            if(ptr->isEmptyForMove(new_pos,board))
                th.push_back(new_pos);
        }
    }
    ptr->set_check(false);
    return th;
}

Board::Board() : black_king(nullptr), white_king(nullptr), board(8, 8) {}

Board::~Board()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            delete board[i][j];
        }
    }
}

bool Board::IsKingInCheck(Color color) const
{
    const ChessPiece* king = (color == Color::WHITE) ? white_king : black_king;
    Position kingPos = king->get_pos();
    const std::vector<ChessPiece*>& opponentPieces = (color == Color::WHITE) ? blackPieces : whitePieces;
    auto checkForCheck = [&kingPos, this](const ChessPiece* opponentPiece) {
        const std::vector<Position> validMoves = GetValidAttacks(opponentPiece->get_pos());
        return std::any_of(validMoves.begin(), validMoves.end(), [&kingPos](const Position& move) {
            return move == kingPos;
        });
    };

    std::vector<std::thread> threads;
    std::vector<bool> results(opponentPieces.size());
    std::mutex resultsMutex;

    for (size_t i = 0; i < opponentPieces.size(); ++i)
    {
        threads.emplace_back([checkForCheck, i, &results, &opponentPieces, &resultsMutex, kingPos]()                             {
            bool result = checkForCheck(opponentPieces[i]);
            std::lock_guard<std::mutex> lock(resultsMutex);
            results[i] = result;
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
    threads.clear();
    return std::any_of(results.begin(), results.end(), [](bool result) {
        return result;
    });
}

bool Board::isValidMove(Position from, Position to) const
{
    ChessPiece * ptr = board[from.getRow()][from.getCol()];
    if(ptr)
    {
        std::vector<Position> validM = GetValidMoves(from);
        for(auto i : validM)
        {
            if(i == to)
            {
                return true;
            }
        }
    }
    return false;
}

void Board::set_piece(Position pos_, Position new_pos)
{

    if (board[pos_.getRow()][pos_.getCol()]->get_name() == 'K')
    {
        if (pos_.getRow() == new_pos.getRow())
        {
            if (pos_.getCol() - 2 == new_pos.getCol())
            {
                board[pos_.getRow()][pos_.getCol()]->setPos(new_pos);
                Position rook_pos(pos_.getRow(), 3);
                board[pos_.getRow()][0]->setPos(rook_pos);
                board[new_pos.getRow()][new_pos.getCol()] = board[pos_.getRow()][pos_.getCol()];
                board[pos_.getRow()][pos_.getCol()] = nullptr;
                board[new_pos.getRow()][new_pos.getCol()]->setIsMoving(true);

                board[rook_pos.getRow()][rook_pos.getCol()] = board[pos_.getRow()][0];
                board[pos_.getRow()][0] = nullptr;
                board[rook_pos.getRow()][rook_pos.getCol()]->setIsMoving(true);
                return;
            } else if (pos_.getCol() + 2 == new_pos.getCol()) {
                board[pos_.getRow()][pos_.getCol()]->setPos(new_pos);
                Position rook_pos(pos_.getRow(), 5);
                board[pos_.getRow()][7]->setPos(rook_pos);
                board[new_pos.getRow()][new_pos.getCol()] = board[pos_.getRow()][pos_.getCol()];
                board[pos_.getRow()][pos_.getCol()] = nullptr;
                board[new_pos.getRow()][new_pos.getCol()]->setIsMoving(true);

                board[rook_pos.getRow()][rook_pos.getCol()] = board[pos_.getRow()][7];
                board[pos_.getRow()][7] = nullptr;
                board[rook_pos.getRow()][rook_pos.getCol()]->setIsMoving(true);
                board[rook_pos.getRow()][rook_pos.getCol()]->setIsMoving(true);

                return;
            }
        }
    }


    board[pos_.getRow()][pos_.getCol()]->setPos(new_pos);
    if (board[new_pos.getRow()][new_pos.getCol()] != nullptr)
    {
        del_from_vector(new_pos, board[pos_.getRow()][pos_.getCol()]->GetColor());
        delete board[new_pos.getRow()][new_pos.getCol()];
    }
    board[new_pos.getRow()][new_pos.getCol()] = board[pos_.getRow()][pos_.getCol()];
    board[pos_.getRow()][pos_.getCol()] = nullptr;
    board[new_pos.getRow()][new_pos.getCol()]->setIsMoving(true);
}


void Board::clearBoard()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if(board[i][j] != nullptr)
            {
                delete board[i][j];
                board[i][j] = nullptr;
            }
        }
    }

    whitePieces.clear();
    blackPieces.clear();

    gameHistory.clear();
}

ChessPiece *Board::createChessPiece(char symbol, Color color, int row, int col)
{
    ChessPiece *piece = nullptr;

    switch (std::toupper(symbol))
    {
    case 'P':
        piece = new Pawn(color, row, col);
        break;
    case 'Q':
        piece = new Queen(color, row, col);
        break;
    case 'R':
        piece = new Rook(color, row, col);
        break;
    case 'H':
        piece = new Horse(color, row, col);
        break;
    case 'B':
        piece = new Bishop(color, row, col);
        break;
    case 'K':
        piece = new King(color, row, col);
        if(color == Color::WHITE) {
            white_king = piece;
        } else {
            black_king = piece;
        }
        break;
    default:
        break;
    }

    if (piece && piece->get_name() != 'K')
    {
        if (color == Color::WHITE)
        {
            whitePieces.push_back(piece);
        } else {
            blackPieces.push_back(piece);
        }
    }

    return piece;
}

bool Board::isEmptyChessPiece(Position pos, Color color) const
{
    if(board[pos.getRow()][pos.getCol()] && color == board[pos.getRow()][pos.getCol()]->GetColor())
    {
        return true;
    }
    return false;
}



bool isPieceChar(char ch)
{
    return (ch == 'P' || ch == 'N' || ch == 'B' || ch == 'R' || ch == 'Q' || ch == 'K' ||
            ch == 'p' || ch == 'n' || ch == 'b' || ch == 'r' || ch == 'q' || ch == 'k');
}

bool isValidPieceCount(const std::string& boardPart)
{
    int count = 0;
    char prevChar = '\0';

    for (char ch : boardPart)
    {
        if (isdigit(ch))
        {
            count += (ch - '0');
            if (prevChar == 'P' || prevChar == 'p')
            {
                if (count > 8)
                {
                    return false;
                }
                count = 0;
            }
        } else if (isPieceChar(ch)) {
            count++;
            if (prevChar == 'P' || prevChar == 'p')
            {
                if (ch == 'N' || ch == 'n')
                {
                    if (count > 2)
                    {
                        return false;
                    }
                }
            }
        } else {
            return false;
        }

        prevChar = ch;
    }

    return true;
}

std::vector<std::string> splitFEN(const std::string& fen)
{
    std::vector<std::string> parts;
    std::istringstream iss(fen);
    std::string part;

    while (getline(iss, part, ' '))
    {
        parts.push_back(part);
    }

    return parts;
}

bool isValidRank(const std::string& rank)
{
    int count = 0;

    for (char ch : rank)
    {
        if (isdigit(ch))
        {
            count += (ch - '0');
        } else if (isPieceChar(ch)) {
            count++;
        } else {
            return false;
        }
    }

    return count == 8;
}

std::vector<std::string> splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

bool isValidFEN(const std::string& fen)
{
    std::vector<std::string> fenParts = splitFEN(fen);
    if (fenParts.size() != 6)
    {
        for (const auto& part : fenParts)
        {
          //  std::cerr << part << std::endl;
        }
        return false;
    }

    std::vector<std::string> ranks = splitString(fenParts[0], '/');
    if (ranks.size() != 8)
    {
        return false;
    }

    for (const auto& rank : ranks)
    {
        if (!isValidRank(rank))
        {
            return false;
        }
    }

    if (!isValidPieceCount(fenParts[0]))
    {
        return false;
    }

    return true;
}

void Board::del_from_vector(Position pos, Color col)
{
    std::vector<ChessPiece *> &pieces = (col == Color::WHITE) ? blackPieces : whitePieces;

    auto it = std::remove_if(pieces.begin(), pieces.end(),
                             [pos](const ChessPiece* piece) {
                                 return piece->get_pos() == pos;
                             });

    if (it != pieces.end())
    {
        pieces.erase(it, pieces.end());
    }

}


void Board::fromFEN(const std::string &filename)
{
    std::ifstream file;
    std::string fen ;
    file.open(filename);
    if (!file.is_open())
    {

        fen = "rhbqkbhr/pppppppp/8/8/8/8/PPPPPPPP/RHBQKBHR";

    } else {
        std::getline(file, fen);
        file.close();
    }
    int row = 7;
    int col = 0;

    for (char c : fen)
    {
        if (c == '/')
        {
            row--;
            col = 0;
        } else if (isdigit(c)) {
            col += (c - '0');
        } else {
            Color pieceColor = islower(c) ? Color::WHITE : Color::BLACK;
            board[row][col] = dynamic_cast<ChessPiece*>(createChessPiece(c, pieceColor, row, col));
            col++;
        }
    }
}

Matrix<ChessPiece *> &Board::getBoard()
{
    return board;
}

std::vector<ChessPiece *> Board::get_opposite_color_pieces(Color color) const
{
    return (color == Color::WHITE) ? blackPieces:whitePieces;
}

bool Board::is_attacked(Position &position, Color color) const
{
    Position kingPos = position;
    const std::vector<ChessPiece*>& opponentPieces = (color == Color::WHITE) ? blackPieces : whitePieces;
    auto checkForCheck = [&kingPos, this](const ChessPiece* opponentPiece) {
        const std::vector<Position> validMoves = GetValidAttacks(opponentPiece->get_pos());
        return std::any_of(validMoves.begin(), validMoves.end(), [&kingPos](const Position& move) {
            return move == kingPos;
        });
    };

    std::vector<std::thread> threads;
    std::vector<bool> results(opponentPieces.size());

    for (size_t i = 0; i < opponentPieces.size(); ++i)
    {
        threads.emplace_back([&checkForCheck, i, &results, &opponentPieces]() {
                results[i] = checkForCheck(opponentPieces[i]);
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    return std::any_of(results.begin(), results.end(), [](bool result) {
        return result;
    });
}

ChessPiece *Board::GetPieceAt(Position pos)
{
    return board[pos.getRow()][pos.getCol()];
}

Board & Board::operator=(const Board &other)
{
    if(this != &other)
    {
        for (auto piece : other.whitePieces)
        {
            auto newPiece = piece->clone();
            this->whitePieces.push_back(newPiece);
            this->board[piece->get_pos().getRow()][piece->get_pos().getCol()] = newPiece;

        }

        for (auto piece : other.blackPieces)
        {
            auto newPiece = piece->clone();
            this->blackPieces.push_back(newPiece);
            this->board[piece->get_pos().getRow()][piece->get_pos().getCol()] = newPiece;

        }
        this->white_king = other.white_king->clone();
        this->black_king = other.black_king->clone();
    }
    return *this;
}

bool Board::check_after_move(Color col, Position from, Position to) const
{
    Board copy;
    copy.operator=(*this);
    copy.set_piece(from, to);
    if(copy.IsKingInCheck(col))
        return true;
    return false;
}

bool Board::is_mate(Color color) const
{

    std::atomic<bool> is_mate_condition(true);

    auto check_moves = [&](const ChessPiece* king, const std::vector<ChessPiece*>& pieces) {
        if (is_mate_condition.load())
        {
            if (!GetValidMoves(king->get_pos()).empty())
            {
                is_mate_condition.store(false);
            } else {
                for (auto piece : pieces)
                {
                    std::vector<Position> validM = GetValidMoves(piece->get_pos());
                    for (auto j : validM)
                    {
                        Board copy;
                        copy.operator=(*this);
                        copy.set_piece(piece->get_pos(), j);
                        if (!copy.IsKingInCheck(color))
                        {
                            is_mate_condition.store(false);
                            return;
                        }
                    }
                }
            }
        }
    };

    if (color == Color::WHITE)
    {
        for (const auto& piece : whitePieces)
        {
            std::thread(check_moves, white_king, std::ref(whitePieces)).join();
        }
    } else {
        for (const auto& piece : blackPieces)
        {
            std::thread(check_moves, black_king, std::ref(blackPieces)).join();
        }
    }
    return is_mate_condition.load();
}




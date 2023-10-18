#pragma once
#include "Game.h"
#include "Square.h"

//
// the classic game of tic tac toe
//

class TicTacToe : public Game
{
public:
    TicTacToe();
    ~TicTacToe();

    // set up the board
    void setUpBoard() override;
    void drawFrame() override;

    Player* checkForWinner() override;

    std::string initialStateString() override;
    std::string stateString() override;
    void setStateString(const std::string &s) override;
    bool actionForEmptyHolder(BitHolder *holder) override;
    bool canBitMoveFrom(Bit*bit, BitHolder *src) override;
    bool canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst) override;
    void stopGame() override;

private:
    Bit *PieceForPlayer(const int playerNumber);
    Player* ownerAt(int index );
    void    scanForMouse();
    Square   _grid[3][3];
};


#include "TicTacToe.h"

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

//
// make an X or an O
//
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class?
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getCurrentPlayer());
    return bit;
}

void TicTacToe::setUpBoard()
{
    setNumberOfPlayers(2);
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            _grid[y][x].initHolder(ImVec2(100*x + 100, 100*y + 100), "square.png", x, y);
        }
    }
    startGame();
}


//
// scan for mouse is temporarily in the actual game class
// this will be moved to a higher up class when the squares have a heirarchy
// we want the event loop to be elsewhere and calling this class, not called by this class
// but this is fine for tic-tac-toe
//
void TicTacToe::scanForMouse()
{
    ImVec2 mousePos = ImGui::GetMousePos();
    mousePos.x -= ImGui::GetWindowPos().x;
    mousePos.y -= ImGui::GetWindowPos().y;

    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            if (_grid[y][x].isMouseOver(mousePos)) {
                if (ImGui::IsMouseClicked(0)) {
                    if (actionForEmptyHolder(&_grid[y][x])) {
                        endTurn();
                    }
                } else {
                    _grid[y][x].setHighlighted(true);
                }
            } else {
                _grid[y][x].setHighlighted(false);
            }
        }
    }    
}

//
// draw the board and then the pieces
// this will also go somewhere else when the heirarchy is set up
//
void TicTacToe::drawFrame() 
{
    scanForMouse();

    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            _grid[y][x].paintSprite();
            if (_grid[y][x].bit()) {
                _grid[y][x].bit()->paintSprite();
            }
        }
    }
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if (holder->bit()) {
        return false;
    }
    Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber());
    if (bit) {
        bit->setPosition(holder->getPosition());
        holder->setBit(bit);
        return true;
    }   
    return false;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            _grid[y][x].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index )
{
    if (!_grid[index/3][index%3].bit()) {
        return nullptr;
    }
    return _grid[index/3][index%3].bit()->getOwner();
}

Player* TicTacToe::checkForWinner()
{
    static const int kWinningTriples[8][3] =  { {0,1,2}, {3,4,5}, {6,7,8},  // rows
                                                {0,3,6}, {1,4,7}, {2,5,8},  // cols
                                                {0,4,8}, {2,4,6} };         // diagonals
    for( int i=0; i<8; i++ ) {
        const int *triple = kWinningTriples[i];
        Player *p = ownerAt(triple[0]);
        if( p && p == ownerAt(triple[1]) && p == ownerAt(triple[2]) )
            return p;
    }
    return nullptr;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString()
{
    std::string s;
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            Bit *bit = _grid[y][x].bit();
            if (bit) {
                s += std::to_string(bit->getOwner()->playerNumber()+1);
            } else {
                s += "0";
            }
        }
    }
    return s;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            int index = y*3 + x;
            int playerNumber = s[index] - '0';
            if (playerNumber) {
                _grid[y][x].setBit( PieceForPlayer(playerNumber-1) );
            } else {
                _grid[y][x].setBit( nullptr );
            }
        }
    }
}


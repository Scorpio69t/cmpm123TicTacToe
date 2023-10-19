#include "ConnectFour.h"

ConnectFour::ConnectFour()
{
}

ConnectFour::~ConnectFour()
{
}

//
// make an X or an O
//
Bit* ConnectFour::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class?
    bit->LoadTextureFromFile(playerNumber == 1 ? "red.png" : "yellow.png");
    bit->setOwner(getCurrentPlayer());
    return bit;
}

void ConnectFour::setUpBoard()
{
    setNumberOfPlayers(2);
    for (int y=0; y<6; y++) {
        for (int x=0; x<7; x++) {
            _grid[y][x].initHolder(ImVec2(100*x + 100, 100*y + 100), "square.png", x, y, y==0, y==5);
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
void ConnectFour::scanForMouse()
{
    ImVec2 mousePos = ImGui::GetMousePos();
    mousePos.x -= ImGui::GetWindowPos().x;
    mousePos.y -= ImGui::GetWindowPos().y;

    for (int y=6; y>=0; y--) {
        for (int x=0; x<7; x++) {
            if (_grid[y][x].isMouseOver(mousePos)) {
                if (ImGui::IsMouseClicked(0)) {
                    if (actionForEmptyHolder(&_grid[y][x])) {
                        
                        
                        endTurn();
                        return;
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
void ConnectFour::drawFrame() 
{
    scanForMouse();

    for (int y=0; y<6; y++) {
        for (int x=0; x<7; x++) {
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
bool ConnectFour::actionForEmptyHolder(BitHolder *holder)
{
    if (holder->bit()) {
        return false;
    }
    Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber());
    if (bit) {
        bit->setPosition(holder->getPosition());
        auto x = holder->getPosition().x;
        auto y = holder->getPosition().y;
        _lastMove = std::to_string(((int)y/100) - 1) + "," + std::to_string(((int)x/100) - 1);
        std::cout << "action for empty holder curr player: " << getCurrentPlayer()->playerNumber() << std::endl;
        holder->setBit(bit);
        return true;
    }   
    return false;
}

bool ConnectFour::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool ConnectFour::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void ConnectFour::stopGame()
{
    for (int y=0; y<6; y++) {
        for (int x=0; x<7; x++) {
            _grid[y][x].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* ConnectFour::ownerAt(int index )
{
    if (!_grid[index/6][index%7].bit()) {
        return nullptr;
    }
    return _grid[index/6][index%7].bit()->getOwner();
}

Player* ConnectFour::checkForWinner()
{
    std::string lastMove = _lastMove;
    const int y = std::stoi(lastMove.substr(0,1));
    const int x = std::stoi(lastMove.substr(2,1));
    int lastPlayer = std::abs(getCurrentPlayer()->playerNumber() - 1);
    std::cout << "check for Winner curr player: " << lastPlayer << std::endl;
    std::cout << "x: " << x << " y: " << y << std::endl;


    //easiest way would be to check if the last move made a connect 4
    //check horizontal
    int count = 1;
    for(int i=x + 1; i < (i + 2 < 7 ? i + 2 : 7); i++) {
        if(_grid[y][i].bit() && _grid[y][i].bit()->getOwner()->playerNumber() == lastPlayer) {
            count++;
        } else {
            break;
        }
    }
    for (int i=x - 1; i > (i - 2 >= 0 ? i - 2 : 0); i--) {
        if(_grid[y][i].bit() && _grid[y][i].bit()->getOwner()->playerNumber() == lastPlayer) {
            count++;
        } else {
            break;
        }
    }

    
    std::cout << "count_horizontal: " << count << std::endl;
    if(count >= 4) {
        std::cout << "player " << getCurrentPlayer()->playerNumber() << " wins" << std::endl;
        return getCurrentPlayer();
    } else {
        count = 1;
    }
    //check for verticals
    for(int i=y + 1; i<(i+2 < 5 ? i + 2 : 5); i++) {
        if(_grid[i][x].bit() && _grid[i][x].bit()->getOwner()->playerNumber() == lastPlayer) {
            count++;
        } else {
            break;
        }
    }
    for(int i=y-1; i<(i-2 >= 0 ? i - 2 : 0); i--) {
        if(_grid[i][x].bit() && _grid[i][x].bit()->getOwner()->playerNumber() == lastPlayer) {
            count++;
        } else {
            break;
        }
    }
    std::cout << "count_vertical: " << count << std::endl;
    if(count >= 4) {
        std::cout << "player " << getCurrentPlayer()->playerNumber() << " wins" << std::endl;
        return getCurrentPlayer();
    } else {
        count = 1;
    }
    //check for diagonals
    //top right = y-1, x+1
    // check for this 3 times
    // then check for bottom left = y+1, x-1
    // check for this 3 times
    for(int i=1; i<4; i++) {
        if(y-i >= 0 && x+i < 7) {
            if(_grid[y-i][x+i].bit() && _grid[y-i][x+i].bit()->getOwner()->playerNumber() == lastPlayer) {
                count++;
            } else {
                break;
            }
        }
    }
    for(int i=1; i<4; i++) {
        if(y+i < 6 && x-i >= 0) {
            if(_grid[y+i][x-i].bit() && _grid[y+i][x-i].bit()->getOwner()->playerNumber() == lastPlayer) {
                count++;
            } else {
                break;
            }
        }
    }
    std::cout << "count_diagonal: " << count << std::endl;
    if(count >= 4) {
        std::cout << "player " << getCurrentPlayer()->playerNumber() << " wins" << std::endl;
        return getCurrentPlayer();
    } else {
        count = 1;
    }
    //top left = y-1, x-1
    // check for this 3 times
    // then check for bottom right = y+1, x+1
    // check for this 3 times
    for(int i=1; i<4; i++) {
        if(y-i >= 0 && x-i >= 0) {
            if(_grid[y-i][x-i].bit() && _grid[y-i][x-i].bit()->getOwner()->playerNumber() == lastPlayer) {
                count++;
            } else {
                break;
            }
        }
    }
    for(int i=1; i<4; i++) {
        if(y+i < 6 && x+i < 7) {
            if(_grid[y+i][x+i].bit() && _grid[y+i][x+i].bit()->getOwner()->playerNumber() == lastPlayer) {
                count++;
            } else {
                break;
            }
        }
    }
    std::cout << "count_diagonal: " << count << std::endl;
    if(count >= 4) {
        std::cout << "player " << getCurrentPlayer()->playerNumber() << " wins" << std::endl;
        return getCurrentPlayer();
    } else {
        count = 1;
    }
    
    return nullptr;
}

//
// state strings
//
std::string ConnectFour::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string ConnectFour::stateString()
{
    std::string s;
    for (int y=0; y<6; y++) {
        for (int x=0; x<7; x++) {
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
void ConnectFour::setStateString(const std::string &s)
{
    for (int y=0; y<6; y++) {
        for (int x=0; x<7; x++) {
            int index = y*6 + x;
            int playerNumber = s[index] - '0';
            if (playerNumber) {
                _grid[y][x].setBit( PieceForPlayer(playerNumber-1) );
            } else {
                _grid[y][x].setBit( nullptr );
            }
        }
    }
}


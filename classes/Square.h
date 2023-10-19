#pragma once

#include "BitHolder.h"

class Square : public BitHolder
{
public:
    Square() : BitHolder() { _column = 0; _row = 0; }
	// initialize the holder with a position, color, and a sprite
	void	initHolder(const ImVec2 &position, const char *spriteName, const int column, const int row, const bool isTop, const bool isBottom);
private:
    int _column;
    int _row;
    bool _isTop;
    bool _isBottom;
};

#pragma once
#include"piece.h"
class line : public piece {
private:


public:
	line();
	line(int xpos, int ypos);
	void MoveSideways(char dir, int grid[10][16]);
	int turn(char dir);
	void draw(int map[10][16]);
	void land(int map[10][16]);
	bool checkCollision(int grid[10][16]);
};
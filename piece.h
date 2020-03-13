#pragma once
class piece {
private:
protected:
	int x;
	int y;
	int spin;
	bool landed;
public:
	piece();
	piece(int xpos, int ypos);
	void move();
	virtual void MoveSideways(char dir, int grid[10][16]);
	virtual int turn(char dir);
	bool isLanded();
	virtual void draw(int map[10][16]);
	int getX();
	int getY();
	virtual void land(int map[10][16]);
	virtual bool checkCollision(int grid[10][16]);
};
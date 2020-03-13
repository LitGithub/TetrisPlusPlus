#include"line.h"
#include<allegro5\allegro_primitives.h>
line::line() {
	x = 0;
	y = 0;
	spin = 1;
	landed = false;
}
line::line(int xpos, int ypos) {
	x = xpos;
	y = ypos;
	landed = false;
	spin = 1;
}
int line::turn(char dir) {
	if (spin == -1 && y < (13 * 50)) {
		spin *= -1;
		return 1;
	}
	//don't spin if too close to right edge
	if (spin == 1 && (x < (7 * 50)))
		spin *= -1;
	return 1;
}
void line::draw(int map[10][16]) {
	if (spin == 1) {
		map[x / 50][y / 50] = 1;
		map[x / 50][(y + 50) / 50] = 1;
		map[x / 50][(y + 100) / 50] = 1;
		map[x / 50][(y + 150) / 50] = 1;
	}
	else if (spin == -1) {
		map[x / 50][y / 50] = 1;
		map[(x + 50) / 50][y / 50] = 1;
		map[(x + 100) / 50][y / 50] = 1;
		map[(x + 150) / 50][y / 50] = 1;
	}
}
//this paints the block's final position // to the actual map before the piece is destroyed
void line::land(int map[10][16]) {
	landed = true;
	if ((spin == 1)) {
		map[x / 50][y / 50] = 10;
		map[x / 50][(y + 50) / 50] = 10;
		map[(x / 50)][(y + 100) / 50] = 10;
		map[x / 50][(y + 150) / 50] = 10;
	}
	if (spin == -1) {
		map[x / 50][(y) / 50] = 10;
		map[(x + 50) / 50][(y) / 50] = 10;
		map[(x + 100) / 50][(y) / 50] = 10;
		map[(x + 150) / 50][(y) / 50] = 10;
	}
}

//moves the piece based on keyboard input 
void line::MoveSideways(char dir, int grid[10][16]) {
	if (dir == 'r' && spin == 1 && x < 500 - 50 && (grid[(x + 50) / 50][y / 50] < 10) && (grid[(x + 50) / 50][(y + 100) / 50] < 10) && (grid[(x + 50) / 50][(y + 200) / 50] < 10))
		x += 50;
	else if (dir == 'l' && x > 0 && (grid[(x - 50) / 50][y / 50] < 10) && (grid[(x - 50) / 50][(y + 100) / 50] < 10) && (grid[(x - 50) / 50][(y + 200) / 50] < 10))
		x -= 50;
	else if (dir == 'd' && y < 800 - 250)
		y += 50;
	else if (dir == 'r' && (spin == -1) && (x + 200 < 500) && (grid[(x + 200) / 50][y / 50] != 10))
		x += 50;
}

bool line::checkCollision(int grid[10][16]) {
	if (spin == 1) { //line piece is vertical
		if (grid[x / 50][(y + 200) / 50] >= 10)
			return true;
		if (y + 200 > 800)
			return true;
		else
			return false;
	}
	else if (spin == -1) { //line piece is horizontal ;
		if (//bottom left corner
			(grid[x / 50][(y + 50) / 50] >= 10) || // second from left corner
			(grid[(x + 50) / 50][(y + 50) / 50] >= 10) || //third from left corner
			(grid[(x + 100) / 50][(y + 50) / 50] >= 10) || //fourth from left corner 
			(grid[(x + 150) / 50][(y + 50) / 50] >= 10) || //bottom right corner
			(grid[(x + 200 - 10) / 50][(y + 50) / 50] >= 10))
			return true;
		if (y + 50 > 800)
			return true;
		else
			return false;
	}
	return false;
}
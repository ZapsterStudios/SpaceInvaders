#include "Enemy.h"

/**
 * todo
 */
Enemy::Enemy(int width, int height, u32 x, u32 y) {
	//
	this->width = width;
	this->height = height;

	//
	this->xPos = x;
	this->yPos = y;
}

/**
 * todo
 */
void Enemy::render(u8 *frame, u32 x, u32 y, int addr) {
	// todo
	if ((x >= this->xPos && x <= this->xPos+this->width) && (y >= this->yPos && y <= this->yPos+this->height)) {
		frame[addr + 0] = 255;	// G
		frame[addr + 1] = 255;	// B
		frame[addr + 2] = 255;	// R
	}
}

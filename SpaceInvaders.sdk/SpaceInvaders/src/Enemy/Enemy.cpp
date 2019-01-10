#include "Enemy.h"

/**
 * todo
 */
Enemy::Enemy() {
	//
	this->xPos = 50;
	this->yPos = 50;
	this->width = 640;
	this->height = 480;
}

/**
 * todo
 */
void Enemy::render(u8 *frame) {
	// todo: pos?
	/*for (int xcoi = 0; xcoi < this->width*3; xcoi += 3) {
		// Set the current pixel address.
		int addr = xcoi;

		// Loop through the height.
		for (int ycoi = 0; ycoi < this->height; ycoi++) {
			// Draw background scene.
			frame[addr + 0] = 0;	// G
			frame[addr + 1] = 255;	// B
			frame[addr + 2] = 0;	// R

			// Jump to next line.
			addr += (1920 * 3);
		}
	}*/

	for (int xcoi = 0; xcoi < this->width*3; xcoi += 3) {
		// Set the current pixel address.
		int addr = xcoi;

		// Loop through the height.
		for (int ycoi = 0; ycoi < this->height; ycoi++) {
			//
			if ((xcoi > 10*3 && xcoi < 630*3) && (ycoi > 10 && ycoi < 470)) {
				frame[addr + 0] = 255;	// G
				frame[addr + 1] = 255;	// B
				frame[addr + 2] = 255;	// R
			}

			// Jump to next line.
			addr += (1920 * 3);
		}
	}
}

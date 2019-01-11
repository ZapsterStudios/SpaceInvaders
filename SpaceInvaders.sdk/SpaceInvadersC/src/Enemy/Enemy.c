// Header
#include "Enemy.h"

// Globals
int enemyState = 0;
int enemyDirection = 1;

// Get the enemy animation state.
u8* enemyImage(Enemy *enemy) {
	// Calculate offset based on enemy row.
	int offset = 0;
	if (enemy->row < 3) offset = 2;
	if (enemy->row >= 3) offset = 4;

	// Return the image at offset + animation.
	return MonsterImages[offset + enemy->animation];
}

// Initialize the enemies.
void initializeEnemies(Enemy *enemies) {
	// Loop through all possible enemies.
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		// Load in the current enemy.
		Enemy *enemy = &enemies[i];

		// Set default sizes.
		enemy->width = ENEMY_SIZE;
		enemy->height = ENEMY_SIZE;
	}
}

// Paint the enemies.
void paintEnemies(Enemy *enemies, u8 *frame) {
	// Loop through all possible enemies.
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		// Load in the current enemy.
		Enemy *enemy = &enemies[i];

		// Get the enemy animation state.
		u8 *state = enemyImage(enemy);

		// Generate the frame address for the starting position.
		int addr = (enemy->xPos * 3) + (enemy->yPos * STRIDE);

		// Loop through the height and set data.
		for (int ycoi = 0; ycoi < enemy->height; ycoi++) {
			// Copy enemy image data into frame.
			memcpy(&frame[addr], &state[ycoi * ENEMY_SIZE * 3], ENEMY_SIZE * 3);

			// Jump to next line.
			addr += STRIDE;
		}
	}
}

// Depaint the enemies.
void depaintEnemies(Enemy *enemies, u8 *frame) {
	// Loop through all possible enemies.
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		// Load in the current enemy.
		Enemy *enemy = &enemies[i];

		// Generate the frame address for the starting position.
		int addr = (enemy->xPos * 3) + (enemy->yPos * STRIDE);

		// Loop through the height and set data.
		for (int ycoi = 0; ycoi < enemy->height; ycoi++) {
			// Draw the background color.
			memset(&frame[addr], BACKGROUND, ENEMY_SIZE * 3);

			// Jump to next line.
			addr += STRIDE;
		}
	}
}

// Position the enemies.
void positionEnemies(Enemy *enemies) {
	// Change offset based on state.
	int curOffset = ENEMY_X_OFFSET + (ENEMY_X_STATE_OFFSET * enemyState);

	// Change state based on direction.
	if (enemyDirection) {
		enemyState++;
	} else {
		enemyState--;
	}

	// Change directions on bounds.
	if (enemyState <= -ENEMY_X_STATES || enemyState >= ENEMY_X_STATES) {
		enemyDirection = ! enemyDirection;
	}

	// Define starting x and y positions.
	u32 x = curOffset, y = ENEMY_Y_OFFSET;

	// Loop through enemies.
	int row = 0;
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		// Load in the current enemy.
		Enemy *enemy = &enemies[i];

		// Set new positions.
		enemy->xPos = x;
		enemy->yPos = y;

		// Set new animation.
		enemy->row = row;
		enemy->animation = ! enemy->animation;

		// Increment x with spacing.
		x += ENEMY_SIZE + ENEMY_SPACE;

		// Jump to next line if equal to columns.
		if ((i+1) % ENEMY_COLS == 0) {
			// Increment the current row.
			row++;

			//
			x = curOffset;
			y += ENEMY_SIZE + ENEMY_SPACE;
		}
	}
}
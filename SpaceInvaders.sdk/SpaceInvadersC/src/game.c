// Includes
#include "game.h"

// Buffer arrays.
u8 buffer[DISPLAY_NUM_FRAMES][MAX_FRAME];
u8 *bufferPointer[DISPLAY_NUM_FRAMES];

// Object arrays.
XTime enemyTimer;
int enemyState = 0, enemyDirection = 1;
Enemy enemies[ENEMY_ROWS*ENEMY_COLS];

// Time reader object.
XTime timeReader;

// todo: fix
u8 enemyImage[(20*20*3)+1] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,5,0,0,16,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,16,0,0,5,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,22,0,0,63,0,0,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,29,0,0,63,0,0,22,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,12,0,0,35,0,0,31,0,0,28,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,28,0,0,31,0,0,35,0,0,12,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,63,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,63,0,0,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,14,0,0,40,0,0,52,0,0,63,0,0,46,0,0,40,0,0,40,0,0,40,0,0,40,0,0,46,0,0,63,0,0,52,0,0,40,0,0,14,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,22,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,22,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,8,0,0,51,0,0,56,0,0,63,0,0,36,0,0,12,0,0,49,0,0,63,0,0,63,0,0,63,0,0,63,0,0,49,0,0,12,0,0,36,0,0,63,0,0,56,0,0,51,0,0,8,0,0,0,0
		,0,0,0,0,10,0,0,63,0,0,63,0,0,63,0,0,29,0,0,0,0,0,45,0,0,63,0,0,63,0,0,63,0,0,63,0,0,45,0,0,0,0,0,29,0,0,63,0,0,63,0,0,63,0,0,10,0,0,0,0
		,0,62,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,62,0
		,0,62,0,0,61,0,0,51,0,0,56,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,56,0,0,51,0,0,61,0,0,62,0
		,0,62,0,0,53,0,0,0,0,0,22,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,63,0,0,22,0,0,0,0,0,53,0,0,62,0
		,0,62,0,0,53,0,0,0,0,0,22,0,0,63,0,0,51,0,0,40,0,0,40,0,0,40,0,0,40,0,0,40,0,0,40,0,0,40,0,0,40,0,0,51,0,0,63,0,0,22,0,0,0,0,0,53,0,0,62,0
		,0,62,0,0,53,0,0,0,0,0,22,0,0,63,0,0,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,29,0,0,63,0,0,22,0,0,0,0,0,53,0,0,62,0
		,0,27,0,0,23,0,0,0,0,0,9,0,0,28,0,0,32,0,0,35,0,0,35,0,0,35,0,0,3,0,0,3,0,0,35,0,0,35,0,0,35,0,0,32,0,0,28,0,0,9,0,0,0,0,0,23,0,0,27,0
		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,63,0,0,63,0,0,63,0,0,6,0,0,6,0,0,63,0,0,63,0,0,63,0,0,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,16,0,0,16,0,0,16,0,0,1,0,0,1,0,0,16,0,0,16,0,0,16,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		,0 // test

};


/**
 * Initialize the game logic and display.
 */
int main()
{
	// Initialize the display.
	initializeDisplay();

	// Render the scene.
	while (1) {
		sleep(1);
		renderScene();
	}

	// Return out.
	return 0;
}

/**
 * todo
 */
int collides(Enemy *enemy, u32 x, u32 y) {
	return (
		x >= enemy->xPos &&
		y >= enemy->yPos &&
		x <= enemy->xPos + enemy->width &&
		y <= enemy->yPos + enemy->height
	);
}

/**
 * Render the scene.
 */
void renderScene()
{
	// Get the current frame.
	u8 *frame = controller.framePtr[controller.curFrame];

	// Update the enemy positions.
	positionEnemies();

	// Set whole frame to background color.
	memset(&frame[0], BACKGROUND, sizeof(u8) * MAX_FRAME);

	// Loop through all possible enemies.
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		// Load in the current enemy.
		Enemy *enemy = &enemies[i];

		// Generate the frame address for the starting position.
		int addr = (enemy->xPos * 3) + (enemy->yPos * STRIDE);

		// Loop through the height and set data.
		for (int ycoi = 0; ycoi < enemy->height; ycoi++) {
			// Copy enemy image data into frame.
			memcpy(&frame[addr], &enemyImage[ycoi * ENEMY_SIZE * 3], ENEMY_SIZE * 3);

			// Jump to next line.
			addr += STRIDE;
		}
	}

	// Flush the frame cache.
	Xil_DCacheFlushRange((unsigned int) frame, MAX_FRAME);
}

/**
 * todo
 */
void positionEnemies() {
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
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		// Set new positions.
		enemies[i].xPos = x;
		enemies[i].yPos = y;

		// Set new sizes.
		enemies[i].width = ENEMY_SIZE;
		enemies[i].height = ENEMY_SIZE;

		// Increment x with spacing.
		x += ENEMY_SIZE + ENEMY_SPACE;

		// Increment y and reset x.
		if ((i+1) % 11 == 0) {
			x = curOffset;
			y += ENEMY_SIZE + ENEMY_SPACE;
		}
	}
}


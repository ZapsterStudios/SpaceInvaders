// Includes
#include "game.h"

// Buffer arrays.
u8 buffer[DISPLAY_NUM_FRAMES][MAX_FRAME];
u8 *bufferPointer[DISPLAY_NUM_FRAMES];

// Object arrays.
Player player;
Enemy enemies[ENEMY_ROWS*ENEMY_COLS];

// todo
int drawBackground = 1;

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

	// Draw the background if needed.
	if (drawBackground) {
		// Draw the background.
		memset(&frame[0], BACKGROUND, sizeof(u8) * MAX_FRAME);

		// Disable background drawing.
		drawBackground = 0;
	}

	// Position and paint enemies.
	depaintEnemies(enemies, frame);
	positionEnemies(enemies);
	paintEnemies(enemies, frame);

	// Position and paint player.
	depaintPlayer(&player, frame);
	positionPlayer(&player);
	paintPlayer(&player, frame);

	// Flush the frame cache.
	Xil_DCacheFlushRange((unsigned int) frame, MAX_FRAME);
}

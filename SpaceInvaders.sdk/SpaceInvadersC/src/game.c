// Includes
#include "game.h"

// Buffer arrays.
u8 buffer[DISPLAY_NUM_FRAMES][MAX_FRAME];
u8 *bufferPointer[DISPLAY_NUM_FRAMES];

//
Enemy enemies[ENEMY_ROWS*ENEMY_COLS];

/**
 * Initialize the game logic and display.
 */
int main()
{
	// Initialize the display.
	initializeDisplay();

	//
	u32 x = 0, y = 0;
	for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
		//
		enemies[i].x = x;
		enemies[i].y = y;

		//
		enemies[i].w = 40;
		enemies[i].h = 40;

		//
		x += 40+10;

		//
		if (i != 0 && i % 11 == 0) {
			y += 40+10;
		}
	}

	// Render the scene.
	while (1) {
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
		(enemy->x >= x) &&
		(enemy->x <= (x+enemy->w)) &&
		(enemy->y >= y) &&
		(enemy->y <= (y+enemy->h))
	);
}

/**
 * Render the scene.
 */
void renderScene()
{
	// Get the current frame.
	u8 *frame = controller.framePtr[controller.curFrame];

	// Loop through the x coordinates.
	for(int xcoi = 0; xcoi < (DISPLAY_WIDTH*3); xcoi += 3)
	{
		// Get the current address.
		int addr = xcoi;

		// Loop through the y coordinates.
		for(int ycoi = 0; ycoi < DISPLAY_HEIGHT; ycoi++)
		{
			// Draw the background scene. todo: only draw once.
			frame[addr + 0] = 0;	// Green
			frame[addr + 1] = 255;	// Blue
			frame[addr + 2] = 0;	// Red

			// Loop through the enemies.
			for (int i = 0; i < ENEMY_ROWS*ENEMY_COLS; i++) {
				// Load in the current enemy.
				Enemy *enemy = &enemies[i];

				// Draw enemy if collides with pixel.
				if (collides(enemy, xcoi/3, ycoi)) {
					frame[addr + 0] = 255;	// Green
					frame[addr + 1] = 255;	// Blue
					frame[addr + 2] = 255;	// Red
				}
			}

			// Jump to next line
			addr += STRIDE;
		}
	}

	// Flush the frame cache.
	Xil_DCacheFlushRange((unsigned int) frame, MAX_FRAME);
}

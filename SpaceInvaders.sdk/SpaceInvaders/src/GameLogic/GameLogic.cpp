#include "GameLogic.h"

/**
 * Initialize the game resources.
 */
GameLogic::GameLogic() {
	// Create status variable.
	int status;

	// Create and initialize new rendering engine.
	status = this->renderer.initialize();
	if (! status) {
		xil_printf("Failed to initialize renderer.\r\n");
		exit(EXIT_FAILURE);
		return;
	}
}

/**
 * Launch the game logic.
 */
int GameLogic::launch() {
	// Add the game scene object.
	this->renderer.addObject(
		new Scene(RENDERER_WIDTH, RENDERER_HEIGHT)
	);

	// Generate the game enemies.
	this->generateEnemies();

	// Start game infinity loop.
	while (1) {
		// TODO: Game logic

		// Render the game.
		this->renderer.render();
	}

	//while(1);

	// Return loop failed.
	return 0;
}

/**
 *
 */
void GameLogic::generateEnemies() {
	//
	u32 width = 40, height = 40;
	u32 x = 10, y = 0;

	//
	for (int i = 0; i < 2; i++) {
		// Create new enemy with given details.
		/*this->enemies[i] = new Enemy(
			width, height,
			x, y
		);*/

		// Add the object to the rendering list.
		this->renderer.addObject(new Enemy(
			width, height,
			x, y
		));

		//
		x += width+6;

		// Jump to next line for every 11th enemies.
		/*if (i == 11) {
			y += height;
		}*/
		/*if (i % 11 == 0) {
			y += height;
		}*/
	}
}

#include "GameLogic.h"

/**
 * Initialize the game resources.
 */
GameLogic::GameLogic() {
	// Create status variable.
	int status;

	// Create and initialize new rendering engine.
	/*status = this->renderer.initialize();
	if (! status) {
		xil_printf("Failed to initialize renderer.\r\n");
		exit(EXIT_FAILURE);
		return;
	}*/
}

/**
 * Launch the game logic.
 */
int GameLogic::launch() {
	// Add the game scene object.
	this->renderer.addObject(
		new Scene(DISPLAY_WIDTH, DISPLAY_HEIGHT)
	);

	// Generate the game enemies.
	this->generateEnemies();

	// Start game infinity loop.
	while (1) {
		// TODO: Game logic

		// Render the game.
		this->renderer.render();
	}

	// Return loop failed.
	return 0;
}

/**
 *
 */
void GameLogic::generateEnemies() {
	//
	//Enemy *enemies[16];

	/*for (int i = 0; i < 10; i++) {
		enemies[i] = new Enemy();
	}*/



	//
}

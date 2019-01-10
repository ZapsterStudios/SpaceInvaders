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

	//
	this->renderer.addObject(new Enemy());

	// Start game infinity loop.
	while (1) {
		// TODO: Game logic

		// Render the game.
		this->renderer.render();
	}

	// Return loop failed.
	return 0;
}
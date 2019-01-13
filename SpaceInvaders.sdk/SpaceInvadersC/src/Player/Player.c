// Header
#include "Player.h"

// Create BTN gpio struct.
XGpio BTNGpio;

// Initialize the player.
void initializePlayer(Player *player) {
	// Set starting position.
	player->position = (DISPLAY_WIDTH / 2) - (PLAYER_SIZE / 2);

	// Initialize the button XGPIO.
	XGpio_Initialize(&BTNGpio, GPIO_BTN_ID);
	XGpio_SetDataDirection(&BTNGpio, BTN_CHANNEL, 0xFF);
}

// Paint the player.
void paintPlayer(Player *player, u8 *frame) {
	// Generate the frame address for the starting position.
	int addr = (player->position * 3) + (PLAYER_Y_OFFSET * STRIDE);

	// Loop through the height and set data.
	for (int ycoi = 0; ycoi < PLAYER_SIZE; ycoi++) {
		// Copy enemy image data into frame.
		memcpy(&frame[addr], &Ship[ycoi * PLAYER_SIZE * 3], PLAYER_SIZE * 3);

		// Jump to next line.
		addr += STRIDE;
	}
}

// Depaint the player.
void depaintPlayer(Player *player, u8 *frame) {
	// Generate the frame address for the starting position.
	int addr = (player->position * 3) + (PLAYER_Y_OFFSET * STRIDE);

	// Loop through the height and set data.
	for (int ycoi = 0; ycoi < PLAYER_SIZE; ycoi++) {
		// Draw the background color.
		memset(&frame[addr], BACKGROUND, PLAYER_SIZE * 3);

		// Jump to next line.
		addr += STRIDE;
	}
}

// Position the enemies.
void positionPlayer(Player *player) {
	// Read in the current button state.
	u8 state = XGpio_DiscreteRead(&BTNGpio, BTN_MASK);

	// Set default next position.
	u32 nextPosition = player->position;

	// Determine the direction.
	if (state == PLAYER_KEY_LEFT) {
		nextPosition += PLAYER_MOVEMENT;
	} else if (state == PLAYER_KEY_RIGHT) {
		nextPosition -= PLAYER_MOVEMENT;
	}

	// Update position if not outside bounds.
	if (nextPosition > 0 && nextPosition < (DISPLAY_WIDTH - PLAYER_SIZE)) {
		player->position = nextPosition;
	}
}

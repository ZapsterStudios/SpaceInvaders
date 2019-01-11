// Header
#include "Player.h"

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
	//
	u32 nextPos = player->position + PLAYER_SIZE;
	player->position = nextPos;

	/*u8 nextPos = player->position + PLAYER_SIZE;
	if (nextPos > (DISPLAY_WIDTH - PLAYER_SIZE)) {
		nextPos = 0;
	}

	// todo: add movement
	player->position = 0;*/
}

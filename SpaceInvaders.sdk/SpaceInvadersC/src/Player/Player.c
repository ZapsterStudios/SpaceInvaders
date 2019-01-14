// Header
#include "Player.h"

// Initialize the player.
void initializePlayer(Player *player) {
	// Set starting position.
	player->position = (DISPLAY_WIDTH / 2) - (PLAYER_SIZE / 2);
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

	// Paint the bullet if exists.
	if (player->bullet != NULL) {
		paintBullet(player->bullet, frame);
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

	// Depaint the bullet if exists.
	if (player->bullet != NULL) {
		depaintBullet(player->bullet, frame);
	}
}

// Position the enemies.
void positionPlayer(Player *player, u8 input) {
	// Set default next position.
	u32 nextPosition = player->position;

	// Determine the direction.
	if (input == PLAYER_KEY_LEFT) {
		nextPosition += PLAYER_MOVEMENT;
	} else if (input == PLAYER_KEY_RIGHT) {
		nextPosition -= PLAYER_MOVEMENT;
	}

	// Update position if not outside bounds.
	if (nextPosition > 0 && nextPosition < (DISPLAY_WIDTH - PLAYER_SIZE)) {
		player->position = nextPosition;
	}

	// Check if shooting and no bullet exists.
	if (player->bullet == NULL && input == PLAYER_KEY_SHOOT) {
		// Allocate space for new bullet.
		player->bullet = malloc(sizeof(Bullet));

		// Set new bullet position.
		player->bullet->direction = 1;
		player->bullet->yPos = PLAYER_Y_OFFSET;
		player->bullet->xPos = player->position + (PLAYER_SIZE / 2);
	}

	// Position bullet if exists.
	if (player->bullet != NULL) {
		player->bullet = positionBullet(player->bullet);
	}
}

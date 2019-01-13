#ifndef SRC_PLAYER_PLAYER_H_
#define SRC_PLAYER_PLAYER_H_

// Includes
#include "xgpio.h"
#include "xil_io.h"
#include "xil_types.h"
#include "../settings.h"

// Images
#include "Resources/Ship.h"

// GPIO
#define BTN_CHANNEL 1
#define BTN_MASK XGPIO_IR_CH1_MASK
#define GPIO_BTN_ID  XPAR_AXI_GPIO_BTN_DEVICE_ID

// Struct
typedef struct {
	// General
	int health;

	// Positions
	u32 position;
} Player;

// Constructors
void positionPlayer(Player *player);
void initializePlayer(Player *player);
void paintPlayer(Player *player, u8 *frame);
void depaintPlayer(Player *player, u8 *frame);

#endif

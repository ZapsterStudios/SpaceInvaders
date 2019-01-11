#ifndef SRC_ENEMY_ENEMY_H_
#define SRC_ENEMY_ENEMY_H_

// Includes
#include "xil_io.h"
#include "xil_types.h"
#include "../settings.h"

// Struct
typedef struct {
	// Sizes
	u32 height;
	u32 width;

	// Positions
	u32 xPos;
	u32 yPos;
} Enemy;

// Constructors
void positionEnemies(Enemy *enemies);
void paintEnemies(Enemy *enemies, u8 *frame);
void depaintEnemies(Enemy *enemies, u8 *frame);

#endif

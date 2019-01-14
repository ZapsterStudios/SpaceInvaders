#ifndef SRC_SHIELD_CORNER_CORNER_H_
#define SRC_SHIELD_CORNER_CORNER_H_

// Includes
#include "xil_io.h"
#include "xil_types.h"

// Images
#include "Resources/State0.h"
#include "Resources/State1.h"
#include "Resources/State2.h"

__attribute__ ((unused))
static u8 *ShieldCornerImages[3] = {
	CornerState0,
	CornerState1,
	CornerState2,
};

// Struct
typedef struct {
	// Sizes
	u32 height;
	u32 width;

	// Positions
	u32 xPos;
	u32 yPos;

	// States
	int health;
} ShieldCorner;

#endif

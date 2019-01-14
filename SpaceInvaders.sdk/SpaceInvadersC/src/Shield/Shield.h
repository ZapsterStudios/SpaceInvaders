#ifndef SRC_SHIELD_SHIELD_H_
#define SRC_SHIELD_SHIELD_H_

// Includes
#include "xil_io.h"
#include "xil_types.h"
#include "Block/Block.h"
#include "Corner/Corner.h"
#include "../Bullet/Bullet.h"

// Struct
typedef struct {
	// Elements
	ShieldBlock *blocks;
	ShieldCorner *corners;
} Shield;

#endif

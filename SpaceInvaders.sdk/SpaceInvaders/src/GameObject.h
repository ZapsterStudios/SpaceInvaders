#include "xil_types.h"

#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

#define STRIDE (1920 * 3)

class GameObject {
	public:
		// Size parameters.
		int width = 0;
		int height = 0;

		// Position parameters.
		u32 xPos = 0;
		u32 yPos = 0;

		// Abstract render function.
		virtual void render(u8 *frame, u32 x, u32 y, int addr) = 0;
};

#endif

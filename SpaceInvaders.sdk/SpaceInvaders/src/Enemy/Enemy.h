#include "../GameObject.h"

#ifndef SRC_ENEMY_ENEMY_H_
#define SRC_ENEMY_ENEMY_H_

class Enemy: public GameObject {
	public:
		Enemy(int width, int height, u32 x, u32 y);
		virtual bool collides(u32 x, u32 y);
		virtual void render(u8 *frame, u32 x, u32 y, int addr);
};

#endif

#include "stdio.h"
#include <stdlib.h>
#include "xil_types.h"
#include "../GameObject.h"
#include "../Scene/Scene.h"
#include "../Enemy/Enemy.h"
#include "../Renderer/Renderer.h"

#ifndef SRC_GAMELOGIC_GAMELOGIC_H_
#define SRC_GAMELOGIC_GAMELOGIC_H_

class GameLogic {
	public:
		GameLogic();
		int launch();
		void generateEnemies();

	protected:
		Renderer renderer;
		//GameObject *player;
		//GameObject *enemies[55+1];
};

#endif

#ifndef GAME_H_
#define GAME_H_

// Includes
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "initializeDisplay.h"
#include "display_ctrl/display_ctrl.h"

// Game objects
#include "Enemy.h"

/* ------------------------------------------------------------ */
/*		    			Game Declarations		    		  	*/
/* ------------------------------------------------------------ */

#define ENEMY_ROWS 5
#define ENEMY_COLS 11

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define MAX_FRAME 	(1920*1080*3)
#define STRIDE 		(1920 * 3)

// Display Size
#define DISPLAY_WIDTH 	640
#define DISPLAY_HEIGHT	480

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void initialize();
void renderScene();
int collides(Enemy *enemy, u32 x, u32 y);

/* ------------------------------------------------------------ */

/************************************************************************/

#endif

// Includes
#include "xil_types.h"
#include "xil_cache.h"
#include "../GameObject.h"
#include "display_ctrl/display_ctrl.h"

// Size defines
#define RENDERER_WIDTH	640
#define RENDERER_HEIGHT	480

// Frame defines
#define STRIDE (1920 * 3)
#define MAX_FRAME (1920 * 1080 * 3)

// Hardware defines
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define DISP_VTC_ID XPAR_VTC_0_DEVICE_ID

#ifndef SRC_RENDERE_RENDERER_H_
#define SRC_RENDERE_RENDERER_H_

class Renderer {
	public:
		Renderer();
		void render();
		int initialize();
		void addObject(GameObject *obj);

	protected:
		XAxiVdma vdma;
		DisplayCtrl controller;
		u8* currentFrame();
		int objectCount = 0;
		GameObject *objects[30];
};

#endif

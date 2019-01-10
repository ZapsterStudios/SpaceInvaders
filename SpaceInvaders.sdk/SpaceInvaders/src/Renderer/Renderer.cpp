#include "Renderer.h"

u8 buffer[DISPLAY_NUM_FRAMES][MAX_FRAME];
u8 *bufferPointer[DISPLAY_NUM_FRAMES];

Renderer::Renderer() {
	//
}

/**
 * Initialize the renderer display.
 */
int Renderer::initialize() {
	// Create status holder.
	int status;

	// Create the VDMA config.
	XAxiVdma_Config *vdmaConfig;

	// Initialize buffer pointer array.
	for (int i = 0; i < DISPLAY_NUM_FRAMES; i++) {
		bufferPointer[i] = buffer[i];
	}

	// Look configuration for VGA VDMA device.
	vdmaConfig = XAxiVdma_LookupConfig(VGA_VDMA_ID);
	if (! vdmaConfig) {
		xil_printf("No video DMA found for ID: %d\r\n", VGA_VDMA_ID);
		return 0;
	}

	// Initialize the VDMA config.
	status = XAxiVdma_CfgInitialize(&this->vdma, vdmaConfig, vdmaConfig->BaseAddress);
	if (status != XST_SUCCESS) {
		xil_printf("VDMA Configuration Initialization failed %d\r\n", status);
		return 0;
	}

	// Initialize the display controller.
	status = DisplayInitialize(&this->controller, &this->vdma, DISP_VTC_ID, DYNCLK_BASEADDR, bufferPointer, STRIDE);
	if (status != XST_SUCCESS) {
		xil_printf("Display controller initialization failed %d\r\n", status);
		return 0;
	}

	// Start the display controller.
	status = DisplayStart(&this->controller);
	if (status != XST_SUCCESS)
	{
		xil_printf("Couldn't start display during initialization %d\r\n", status);
		return 0;
	}

	// Return success.
	return 1;
}

/**
 * Return the current controller frame.
 */
u8* Renderer::currentFrame() {
	return this->controller.framePtr[this->controller.curFrame];
}

/**
 * Render the game objects.
 */
void Renderer::render() {
	// Get the current controller frame.
	u8 *frame = this->currentFrame();

	//
	/*for (int xcoi = 0; xcoi < RENDERER_WIDTH*3; xcoi += 3) {
		//
		int addr = xcoi;

		//
		for (int ycoi = 0; ycoi < RENDERER_HEIGHT; ycoi++) {
			//
			frame[addr + 0] = 255;	// G
			frame[addr + 1] = 0;	// B
			frame[addr + 2] = 0;	// R

			//
			if ((xcoi > 10*3 && xcoi < 630*3) && (ycoi > 10 && ycoi < 470)) {
				frame[addr + 0] = 255;	// G
				frame[addr + 1] = 255;	// B
				frame[addr + 2] = 255;	// R
			}

			//
			addr += STRIDE;
		}
	}*/

	// Loop through the game objects.
	for (int i = 0; i < this->objectCount; i++) {
		// Render the current object.
		this->objects[i]->render(frame);
	}

	// Flush the cache range.
	Xil_DCacheFlushRange((unsigned int) bufferPointer, MAX_FRAME);
}

/**
 * Add a object to the render list.
 */
void Renderer::addObject(GameObject *obj) {
	this->objects[this->objectCount] = obj;
	this->objectCount++;
}

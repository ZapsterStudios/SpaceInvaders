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

	// Loop through the width timed with color values.
	for (int xcoi = 0; xcoi < RENDERER_WIDTH*3; xcoi += 3) {
		// Set the upcoming pixel address.
		int addr = xcoi;

		// Loop through the screen height.
		for (int ycoi = 0; ycoi < RENDERER_HEIGHT; ycoi++) {
			// Define formatted x and y pos.
			u32 x = xcoi/3, y = ycoi;

			// Loop through the game objects.
			for (int i = 0; i < this->objectCount; i++) {
				// Get the current loop object.
				GameObject *obj = this->objects[i];

				// Check if object should be rendered in range.
				if (obj->collides(x, y)) {
					obj->render(frame, x, y, addr);
				}

			}

			// Jump to next line.
			addr += STRIDE;
		}
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

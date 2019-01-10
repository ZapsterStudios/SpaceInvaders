#include "Scene.h"

/**
 * todo
 */
Scene::Scene(int width, int height) {
	this->width = width;
	this->height = height;
}

/**
 * todo
 */
void Scene::render(u8 *frame) {
	// Return out if shouldn't re-render.
	if (! this->shouldRerender) {
		return;
	}

	// Render the background scene.
	for (int xcoi = 0; xcoi < this->width*3; xcoi += 3) {
		// Set the current pixel address.
		int addr = xcoi;

		// Loop through the height.
		for (int ycoi = 0; ycoi < this->height; ycoi++) {
			// Draw background scene.
			frame[addr + 0] = 0;	// G
			frame[addr + 1] = 0;	// B
			frame[addr + 2] = 255;	// R

			// Jump to next line.
			addr += STRIDE;
		}
	}

	//
	this->shouldRerender = 0;
}

#include "Scene.h"

/**
 * todo
 */
Scene::Scene(int width, int height) {
	//
	this->width = width;
	this->height = height;
}

/**
 * todo
 */
void Scene::render(u8 *frame, u32 x, u32 y, int addr) {
	// Render the background scene.
	frame[addr + 0] = 0;	// G
	frame[addr + 1] = 0;	// B
	frame[addr + 2] = 255;	// R
}

/**
 * todo
 */
bool Scene::collides(u32 x, u32 y) {
	return true;
}

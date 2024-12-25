#include <iostream>
#include "utility.h"
#include "image.h"
#include "hittable.h"
#include "scene.h"
#include "sphere.h"
#include "camera.h"

int main() {
	// Scene Settings
	scene world;
	world.add(make_shared<sphere>(point(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point(0,-100.5,-1), 100));

	camera cam;
	cam.render(world);
}

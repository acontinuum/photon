#include <iostream>
#include "utility.h"

int main() {
	// Scene Settings
	scene world;
	world.add(make_shared<sphere>(point(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point(0,-100.5,-1), 100));

	camera cam;
	progress_bar bar;
	
	cam.render(world, bar);
}

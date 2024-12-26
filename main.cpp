#include <iostream>
#include "utility.h"

int main() {
	// Scene Settings
	scene world;
	
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), .3);

    world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, material_right));

	camera cam;
	cam.samples = 10;
	cam.bounces = 10;
	
	progress_bar bar;
	
	cam.render(world, bar);
}

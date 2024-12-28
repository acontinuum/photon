#include <iostream>
#include "lib/utility.h"

int main() {
	// Scene Settings
	scene world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0);

    world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<triangle>(point( -1, 0.0, -1.2), point( -1, 1, -1.2), point( 0.0, 1, -1.2), material_center));
    world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, material_right));


	camera cam;
	cam.samples = 5;
	cam.bounces = 5;
	cam.camera_position = point(0, 0, 0);
	cam.camera_direction = unit_vector(vec3(0, 0, 1));
	progress_bar bar;
	
	cam.render(world, bar);
}

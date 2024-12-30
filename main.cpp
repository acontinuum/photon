#include <iostream>
#include "lib/utility.h"

void load_cover_scene(scene& world, camera& cam) {
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double(0, 1);
            point center(a + 0.9*random_double(0, 1), 0.2, b + 0.9*random_double(0, 1));

            if ((center - point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random(0, 1) * color::random(0, 1);
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<diffuse_light>(color(4, 4, 4));
    world.add(make_shared<sphere>(point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<dielectric>(1.5);
    world.add_triangles(obj_to_triangles("monke.obj", point(4, 1, 0), material3));

    cam.fov = 90;
    cam.camera_position = point(13, 2, 3);
    cam.camera_direction = vec3(13, 2, 3);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focal_distance = 10.0;
}

void load_triangle_scene(scene& world, camera& cam) {
    auto material = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<triangle>(point(-3, 3, -4), point(3, 3, -4), point(0, -3, -4), material));

    cam.fov = 90;
    cam.camera_position = point(0, 0, 0);
    cam.camera_direction = vec3(0, 0, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 3;
}

void load_box_scene(scene& world, camera& cam) {
    auto material = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add_triangles(obj_to_triangles("boxopen.obj", point(0, 0, -4), material));

    auto material1 = make_shared<diffuse_light>(color(4, 4, 4));
    world.add(make_shared<sphere>(point(0, 0, -3.9), .5, material1));

    cam.fov = 90;
    cam.camera_position = point(0, 0, 0);
    cam.camera_direction = vec3(0, 0, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 1;
}

void load_monke_scene(scene& world, camera& cam) {
    auto material = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add_triangles(obj_to_triangles("monke.obj", point(0, 0, -4), material));

    cam.fov = 90;
    cam.camera_position = point(0, 0, 0);
    cam.camera_direction = vec3(0, 0, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 1;
}

int main() {
	// Scene Settings
	scene world;
    camera cam;
    
    load_cover_scene(world, cam);

	world = scene(make_shared<bvh_node>(world));
    
    // Render Settings
    cam.image_width  = 320;
	cam.image_height = 160;
    cam.samples = 10;
    cam.bounces = 10;

	progress_bar bar;
	
	cam.render(world, bar);
}

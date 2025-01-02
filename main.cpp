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

    auto material1 = make_shared<dielectric>(1.75, color(.9, 1, 1));
    world.add(make_shared<sphere>(point(4, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(.3, .3, .3), .2);
    world.add(make_shared<sphere>(point(0, 1, 0), 1.0, material3));
    

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
    world.add_triangles(obj_to_triangles("test_objects/boxopen.obj", point(0, 0, -4), material));

    cam.fov = 90;
    cam.camera_position = point(0, 0, 0);
    cam.camera_direction = vec3(0, 0, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 1;
}

void load_monke_scene(scene& world, camera& cam) {
    auto material = make_shared<lambertian>(color(0.2, 0.2, 0.4));
    world.add_triangles(obj_to_triangles("test_objects/monke.obj", point(0, 0, -4), material));

    cam.fov = 90;
    cam.camera_position = point(0, 0, 0);
    cam.camera_direction = vec3(0, 0, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 1;
}

void load_dragon_scene(scene& world, camera& cam) {
    auto glass_mat = make_shared<dielectric>(1.51, color(1, 1, 1));
    world.add_triangles(obj_to_triangles("stanford_dragon/dragon.obj", point(0, 0, -16), glass_mat));

    auto left_mat = make_shared<lambertian>(color(0.3, 0.8, 0.3));
    world.add_triangles(obj_to_triangles("stanford_dragon/wallleft.obj", point(0, 0, -16), left_mat));

    auto right_mat = make_shared<lambertian>(color(0.3, 0.3, 0.8));
    world.add_triangles(obj_to_triangles("stanford_dragon/wallright.obj", point(0, 0, -16), right_mat));

    auto top_mat = make_shared<lambertian>(color(0, 0, 0));
    world.add_triangles(obj_to_triangles("stanford_dragon/walltop.obj", point(0, 0, -16), top_mat));

    auto bottom_mat = make_shared<lambertian>(color(.8, .8, .8));
    world.add_triangles(obj_to_triangles("stanford_dragon/wallbottom.obj", point(0, 0, -16), bottom_mat));

    auto back_mat = make_shared<lambertian>(color(0.8, 0.3, 0.3));
    world.add_triangles(obj_to_triangles("stanford_dragon/wallback.obj", point(0, 0, -16), back_mat));

    auto front_mat = make_shared<diffuse_light>(color(6, 6, 6));
    //world.add_triangles(obj_to_triangles("stanford_dragon/wallback.obj", point(0, 0, 1), front_mat));


    world.add(make_shared<sphere>(point(0, 0, 50.01), 50, front_mat));

    cam.fov = 55;
    cam.camera_position = point(.05, 0, 0);
    cam.camera_direction = vec3(0, 0, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 1;
}

void load_miatta_scene(scene& world, camera& cam) {
    // Materials
    auto red_body_mat = make_shared<metal>(color(.8, .1, .1), .1);
    auto headlights_mat = make_shared<diffuse_light>(color(3, 3, 3));
    auto backlights_mat = make_shared<diffuse_light>(color(3, 1, 1));
    auto glass_mat = make_shared<dielectric>(1.5, color(1, 1, 1));
    auto internal_block_mat = make_shared<lambertian>(color(0, 0, 0));
    auto podium_mat = make_shared<lambertian>(color(.8, .8, .8));
    auto pop_up_mat = make_shared<lambertian>(color(.5, .5, .5));
    auto upholstery_mat = make_shared<lambertian>(color(.6, .6, .6));
    auto wheel_mat = make_shared<lambertian>(color(.7, .7, .7));
    auto rims_mat = make_shared<metal>(color(.3, .3, .3), .1);
    auto mirror_mat = make_shared<metal>(color(.2, .2, .2), 0);
    auto highlights_mat = make_shared<metal>(color(.9, .25, .08), .3);

    world.add_triangles(obj_to_triangles("miatta/Body.obj", vec3(0, 0, 0), red_body_mat));
    world.add_triangles(obj_to_triangles("miatta/Headlights.obj", vec3(0, 0, 0), headlights_mat));
    world.add_triangles(obj_to_triangles("miatta/BackLights.obj", vec3(0, 0, 0), backlights_mat));
    world.add_triangles(obj_to_triangles("miatta/Glass.obj", vec3(0, 0, 0), glass_mat));
    world.add_triangles(obj_to_triangles("miatta/InternalBlock.obj", vec3(0, 0, 0), internal_block_mat));
    world.add_triangles(obj_to_triangles("miatta/Podium.obj", vec3(0, 0, 0), podium_mat));
    world.add_triangles(obj_to_triangles("miatta/PopUpsBody.obj", vec3(0, 0, 0), pop_up_mat));
    world.add_triangles(obj_to_triangles("miatta/Upholstery.obj", vec3(0, 0, 0), upholstery_mat));
    world.add_triangles(obj_to_triangles("miatta/Wheel Hubs.obj", vec3(0, 0, 0), rims_mat));
    world.add_triangles(obj_to_triangles("miatta/Wheels.obj", vec3(0, 0, 0), wheel_mat));
    world.add_triangles(obj_to_triangles("miatta/Mirrors.obj", vec3(0, 0, 0), mirror_mat));
    world.add_triangles(obj_to_triangles("miatta/Highlights.obj", vec3(0, 0, 0), highlights_mat));

    cam.fov = 35;
    cam.camera_position = point(0, 0, 0);
    cam.camera_direction = vec3(0, .3, 1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focal_distance = 12;
}

int main() {
	// Scene Settings
	scene world;  
    camera cam;
    
    load_miatta_scene(world, cam);

	world = scene(make_shared<bvh_node>(world));
    
    // Render Settings
    /*
    cam.image_width  = 1920;
	cam.image_height = 1080;
    cam.samples = 400;
    cam.bounces = 20;
    */
    // Test Render Settings
    cam.image_width  = 320;
	cam.image_height = 180;
    cam.samples = 10;
    cam.bounces = 20;

	progress_bar bar;
	
	cam.render(world, bar, 40, 30);
}

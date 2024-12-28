#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include <ctime> 
#include <iostream>

#include "hittable.h"
#include "progress.h"
#include "material.h"

class camera {
	public:
		int image_width = 400;
		int image_height = 200;
		
		double fov = 90;

		double defocus_angle = 0;
		double focal_distance = 1;

		vec3 vup = vec3(0, 1, 0);
		point camera_position;
		vec3 camera_direction;
		
		int samples = 1;
		int bounces = 1;
		int total_pixels;
		
		void render(const hittable& world, progress_bar bar) {
			initialize();
						
			image render(image_width, image_height);
			auto start = std::chrono::system_clock::now();

			for (int j = 0; j < image_height; j++) {
		        for (int i = 0; i < image_width; i++) {
		            auto pixel_position = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
					auto ray_direction = pixel_position - camera_position;
					ray r(camera_position, ray_direction);
					
					color pixel_color = color(0, 0, 0);

					for(int n = 0; n < samples; n++) {
						ray r = get_ray(i, j);
						pixel_color += ray_color(r, bounces, world);
					}
					
		            render.addPixel(i, j, pixel_color/samples);
		            bar.update(float((j*image_width + i)) / total_pixels + .01);
		        }
			}
			bar.end();

			auto end = std::chrono::system_clock::now();
			 
		    std::chrono::duration<double> elapsed_seconds = end-start;
		    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		 
		    std::cout <<  "Elapsed time: " << elapsed_seconds.count() << "s"<< std::endl;
			
			render.saveImage("Render.png");}
			
	private:
		double aspect_ratio;
		point pixel00_loc;
		vec3 pixel_delta_u;
		vec3 pixel_delta_v;
		vec3 u, v, w; // Local axis vectors
		
		vec3   defocus_disk_u;
		vec3   defocus_disk_v; 
			
		void initialize() {
			total_pixels = image_width * image_height;
		
			aspect_ratio = image_width/image_height;

			auto theta = deg_to_rad(fov);
			auto h = std::tan(theta/2);
			
			auto viewport_height = 2 * h * focal_distance;
			auto viewport_width = viewport_height * aspect_ratio;

			point camera_position = point(0, 0, 0);

			w = unit_vector(camera_direction);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			auto viewport_u = viewport_width * u;
			auto viewport_v = viewport_height * -v;

			pixel_delta_u = viewport_u / image_width;
			pixel_delta_v = viewport_v / image_height;

			auto viewport_upper_left = camera_position - focal_distance * w - viewport_u/2 - viewport_v/2;
			pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	
	        auto defocus_radius = focal_distance * std::tan(deg_to_rad(defocus_angle / 2));
	        defocus_disk_u = u * defocus_radius;
	        defocus_disk_v = v * defocus_radius;
		}

		color ray_color(const ray& r, int bounces_left, const hittable& world) const {
			if(bounces_left <= 0)
				return color(0, 0, 0);

			hit_record rec;
			
		    if (world.hit(r, interval(0.001, infinity), rec)) {
				ray scattered;
				color attenuation;
				if (rec.mat ->scatter(r, rec, attenuation, scattered))
					return attenuation * ray_color(scattered, bounces_left-1, world);
				std::cout << "scatter failed" <<std::endl;
				return color(0, 0, 0);
		    }
    
			vec3 unit_dir = unit_vector(r.direction());
			auto a = .5*(unit_dir.y() + 1.0);
			return (1 - a)*color(1, 1, 1) + a*color(.5, .7, 1);
		}

		point defocus_disk_sample() const {
		    // Returns a random point in the camera defocus disk.
		    auto p = random_in_unit_disk();
		    return camera_position + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
		}

		ray get_ray(int i, int j) const {
			auto offset = sample_square();

			auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

			auto ray_origin = (defocus_angle <= 0) ? camera_position : defocus_disk_sample();
			auto ray_direction = pixel_sample - ray_origin;

			return ray(ray_origin, ray_direction);
		}

		vec3 sample_square() const {
			return vec3(random_double(-.5, .5), random_double(-.5, .5), 0);
		}
};

#endif

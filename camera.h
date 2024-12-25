#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include <ctime>  

#include "hittable.h"
#include "progress.h"

class camera {
	public:
		int image_width = 320;
		int image_height = 160;
		int samples = 3;
		int bounces = 3;
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
			
			render.saveImage("Render2.png");}
			
	private:
		double aspect_ratio;
		point camera_position;
		point pixel00_loc;
		vec3 pixel_delta_u;
		vec3 pixel_delta_v;
			
		void initialize() {
			total_pixels = image_width * image_height;
		
			aspect_ratio = image_width/image_height;

			auto focal_length = 1.0;
			
			auto viewport_height = 2;
			auto viewport_width = viewport_height * aspect_ratio;

			point camera_position = point(0, 0, 0);

			auto viewport_u = vec3(viewport_width, 0, 0);
			auto viewport_v = vec3(0, -viewport_height, 0);

			pixel_delta_u = viewport_u / image_width;
			pixel_delta_v = viewport_v / image_height;

			auto viewport_upper_left = camera_position - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
			pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
		}

		color ray_color(const ray& r, int bounces_left, const hittable& world) const {
			if(bounces_left <= 0)
				return color(0, 0, 0);

			hit_record rec;
			
		    if (world.hit(r, interval(0.001, infinity), rec)) {
				vec3 direction = rec.normal + random_unit_vector();
				return 0.5*ray_color(ray(rec.p, direction), bounces_left - 1, world);
		    }
    
			vec3 unit_dir = unit_vector(r.direction());
			auto a = .5*(unit_dir.y() + 1.0);
			return (1 - a)*color(1, 1, 1) + a*color(.5, .7, 1);
		}

		ray get_ray(int i, int j) const {
			auto offset = sample_square();

			auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
			auto ray_direction = pixel_sample - camera_position;

			return ray(camera_position, ray_direction);
		}

		vec3 sample_square() const {
			return vec3(random_double(-.5, .5), random_double(-.5, .5), 0);
		}
};

#endif

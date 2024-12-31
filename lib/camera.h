#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include <ctime> 
#include <iostream>
#include <thread>

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
		point camera_position = point(0, 0, 0);
		vec3 camera_direction = point(0, 0, 1);
		
		int samples = 1;
		int bounces = 1;
		int total_pixels;
		
		void render(const hittable& world, progress_bar bar, int thread_axis_size_x, int thread_axis_size_y) {
			initialize();

			int x_count = image_width/thread_axis_size_x; // Number of chunks along x axis
			int y_count = image_height/thread_axis_size_y; // Number of chunks along y axis

			image render(image_width, image_height);
			auto start = std::chrono::system_clock::now();
			std::vector<std::thread> threads;
			for(int j = 0; j < y_count; j++) { 
				for(int i = 0; i < x_count; i++) {
					int x_start = thread_axis_size_x * i ;
					int y_start = thread_axis_size_y * j;
					int x_end = x_start + thread_axis_size_x - 1;
					int y_end = y_start + thread_axis_size_y - 1;
					std::cout << "Creating thread from x:" << x_start << " ->" << x_end << "and y:" << y_start << "->" << y_end << std::endl;

					threads.emplace_back([this, x_start, x_end, y_start, y_end, &render, &world]() {
                this->render_chunk(x_start, x_end, y_start, y_end, render, world);
					});
				}
			}

			for (auto& t : threads) {
				t.join();
			}

			auto end = std::chrono::system_clock::now();
			 
		    std::chrono::duration<double> elapsed_seconds = end-start;
		    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		 
		    std::cout <<  "Elapsed time: " << elapsed_seconds.count() << "s"<< std::endl;
			
			render.saveImage("Multithread.png");
		}
			
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
				return rec.mat->emitted();
		    }

			return background(r);
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

		color background(const ray& r) const {
			
			vec3 unit_dir = unit_vector(r.direction());
			auto a = .5*(unit_dir.y() + 1.0);
			return (1 - a)*color(1, 1, 1) + a*color(.5, .7, 1); 
			//return color(0, 0, 0);
		}

		void render_chunk(int x_start, int x_end, int y_start, int y_end, image& render, const hittable& world) {
			for (int j = y_start; j <= y_end; j++) {
				for (int i = x_start; i <= x_end; i++) {					
					color pixel_color = color(0, 0, 0);

					for(int n = 0; n < samples; n++) {
						ray r = get_ray(i, j);
						pixel_color += ray_color(r, bounces, world);
					}
					
					render.addPixel(i, j, pixel_color/samples);
				}
			}

			std::cout << "Chunk rendered" << std::endl;
		}
};

#endif

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
	public:
		int image_width = 320;
		int image_height = 160;
	
		void render(const hittable& world) {
			initialize();

			image render(image_width, image_height);

			for (int j = 0; j < image_height; j++) {
		        for (int i = 0; i < image_width; i++) {
		            auto pixel_position = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
					auto ray_direction = pixel_position - camera_position;
					ray r(camera_position, ray_direction);

					color pixel_color = ray_color(r, world);
		            render.addPixel(i, j, pixel_color);
		        }
			}

			render.saveImage("Render.png");}
			
	private:
		double aspect_ratio;
		point camera_position;
		point pixel00_loc;
		vec3 pixel_delta_u;
		vec3 pixel_delta_v;
			
		void initialize() {
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

		color ray_color(const ray& r, const hittable& world) const {
			hit_record rec;
		    if (world.hit(r, interval(0, infinity), rec)) {
				return 0.5*(rec.normal + color(1, 1, 1));
		    }
    
			vec3 unit_dir = unit_vector(r.direction());
			auto a = .5*(unit_dir.y() + 1.0);
			return (1 - a)*color(1, 1, 1) + a*color(.5, .7, 1);
		}
};

#endif

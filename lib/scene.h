#ifndef SCENE_H
#define SCENE_H

#include "hittable.h"
#include "aabb.h"
#include <vector>

class scene : public hittable {
	public:
		std::vector<shared_ptr<hittable>> objects;
	
		scene() {}
		scene(shared_ptr<hittable> object) { add(object); }

		void clear() { objects.clear(); }

		void add(shared_ptr<hittable> object) {
			objects.push_back(object);
			bbox = aabb(bbox, object->bounding_box()); // Update bounding box with current box and box of new object to check if the intervals are increased if the new object is out of bounds of the current bbox
		}	

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
			hit_record temp_rec;

			bool hit_anything = false;
			auto closest_hit = ray_t.max;

			for(const auto& object : objects) {
				if(object->hit(r, interval(ray_t.min, closest_hit), temp_rec)) {
					hit_anything = true;
					closest_hit = temp_rec.t;
					rec = temp_rec;
				}
			}
			
			return hit_anything;
		}

		aabb bounding_box() const override { return bbox; }
	private:
		aabb bbox; // Top level bounding box
};

#endif

#ifndef HITTABLE_H
#define HITTABLE_H

class hit_record {
	public:
		point p;
		vec3 normal;
		double t;
		bool front_face;
		// Compute if outward normal means ray is hitting a front face or not.
		// Compute the ray normal. Outward must be unit vector.
		void set_face_normal(const ray& r, const vec3& outward_normal) {
			front_face =  dot(r.direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
};

class hittable {
	public:
		virtual ~hittable() = default;

		virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>

#include "hittable.h"
#include "vector.h"

class triangle : public hittable {
    public:
        triangle(const point& a, const point& b, const point& c, shared_ptr<material> mat) : a(a), b(b), c(c), mat(mat) {}

		bool hit(const ray& r, interval ray_t, hit_record& rec) const override { // REFERENCE - https://courses.cs.washington.edu/courses/cse457/04sp/lectures/triangle_intersection.pdf
            // Compute triangle plane
            vec3 outward_normal = unit_vector(cross((b - a), (c - a)));
            double d = dot(outward_normal, a);

            // Find plane intersection
            if(dot(outward_normal, r.direction()) < 0) // If the dot is 0, the ray is parallel and will never intersect.
                return false; 
            float t = (d - dot(outward_normal, r.origin()))/dot(outward_normal, r.direction());
            point q = r.at(t);

            // Check if intersection is not in triangle
            if(dot(cross((b - a), (q - a)), outward_normal) < 0 || dot(cross((c - b), (q - b)), outward_normal) < 0 || dot(cross((a - c), (q - c)), outward_normal) < 0) 
                return false;

            rec.t = t;
	        rec.p = r.at(rec.t);
			rec.set_face_normal(r, outward_normal);
			rec.mat = mat;
	        return true;
		}

    private:
        point a;
        point b;
        point c;
        shared_ptr<material> mat;
};

#endif
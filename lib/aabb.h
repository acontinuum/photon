#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "vector.h"

class aabb {
    public:
        interval x, y, z;

        aabb() {}

        aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}

        aabb(const point& a, const point& b) {
            x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]); // Find smaller value and set interval based off that
            y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

            if(x.size() < .001) { // Make sure that a bbox axis with width 0 isnt created
                x = x.expand(.01);
            }

            if(y.size() < .001) {
                y = y.expand(.01);
            }
            
            if(z.size() < .001) {
                z = z.expand(.01);
            }
            //std::cout << "(" << x.min << ", " << x.max << ") (" << y.min << ", "<< y.max << ") (" << z.min << ", "<< z.max << ")" << std::endl;
        }

        aabb(const aabb& a, const aabb& b) {
            x = interval(a.x, b.x);
            y = interval(a.y, b.y);
            z = interval(a.z, b.z);
        }

        const interval& axis_interval(int n) const { // Getter
            if(n == 1) return y;
            if(n == 2) return z;
            return x;
        }

        bool hit(const ray& r, interval ray_t) const {
            const point& ray_orig = r.origin();
            const vec3& ray_dir = r.direction();

            for(int axis = 0; axis < 3; axis++) { // Loop through axes
                const interval& ax = axis_interval(axis);
                const double adinv = 1/ray_dir[axis];

                auto t0 = (ax.min - ray_orig[axis]) * adinv; // Compute intersection points for 
                auto t1 = (ax.max - ray_orig[axis]) * adinv; // The ray and the axis interval

                if (t0 < t1) {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                } else {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }

                if (ray_t.max <= ray_t.min)
                    return false;
            }
            return true;
        }

        int longest_axis() const {
            if (x.size() > y.size())
                return x.size() > z.size() ? 0 : 2;
            else
                return y.size() > z.size() ? 1 : 2; // return index of longest axis
        }
        
        static const aabb empty, universe;
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif
#ifndef RAY_H
#define RAY_H

#include "vector.h"

class ray {
	public:	
		ray() {}

		ray(const point& origin, const vec3& direction) : orig(origin), dir(direction) {}

		const point& origin() const { return orig; }
		const vec3& direction() const { return dir; }

		point at(double t) const {
			return orig + t*dir;
		}
			
	private:
		point orig;
		vec3 dir;
};

#endif

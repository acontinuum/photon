#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
	public:
		double min, max;

		interval() : min(+infinity), max(-infinity) {} // Default init is invalid
		interval(double min, double max) : min(min), max(max) {}

		interval(const interval& a, const interval& b) {
			min = a.min <= b.min ? a.min : b.min;
			max = a.max >= b.max ? a.max : b.max;
    	} // Create an interval with min of the smallest min and max of the largest max

		double size() const {
			return max - min;
		}	

		bool contains(double x) const {
			return min <= x && x <=max;
		}

		bool surrounds(double x) const {
			return min < x && x < max;
		}

		double clamp(double x) const {
			if(x < min) return min;
			if(x > max) return max;
			return x;
		}

		interval expand(double delta) const {
			auto pad = delta/2;
			return interval(min - pad, max + pad);
		}

		static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif

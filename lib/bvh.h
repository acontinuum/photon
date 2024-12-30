#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "scene.h"
#include "utility.h"

#include <algorithm>

class bvh_node : public hittable {
    public:
        bvh_node(scene list) : bvh_node(list.objects, 0, list.objects.size()) {}

        bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {
            bbox = aabb::empty; // Create bbox and add objects

            for (size_t object_index=start; object_index < end; object_index++)
                bbox = aabb(bbox, objects[object_index]->bounding_box());

            int axis = bbox.longest_axis();

            auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare; // Choose compare based on int axis

            size_t object_span = end - start; // Compute number of objects

            if (object_span == 1) {
                left = right = objects[start];
            } else if (object_span == 2) {
                left = objects[start];
                right = objects[start+1];
            } else {
                std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator); // If more than 2 objects sort based on axis comparator chosen by rng

                auto mid = start + object_span/2;
                left = make_shared<bvh_node>(objects, start, mid);
                right = make_shared<bvh_node>(objects, mid, end);
            }

        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            if(!bbox.hit(r, ray_t))
                return false;

            bool hit_left = left->hit(r, ray_t, rec); // If it hit the node, check subnodes- recursive down tree
            bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec); // if it hit the left, use rec.t to check for both being hit and a closer hit on the right.

            return hit_left || hit_right;
        }

        aabb bounding_box() const override { return bbox; }
    private:
        shared_ptr<hittable> left; // Left node
        shared_ptr<hittable> right; // Right node
        aabb bbox; // bbox for node

        static bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index) {
            auto a_axis_interval = a->bounding_box().axis_interval(axis_index); // Get axis intervals per axis index
            auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
            return a_axis_interval.min < b_axis_interval.min; // Return true if the first interval min is less than the second interval min
        }

        static bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return box_compare(a, b, 0);
        }

        static bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return box_compare(a, b, 1);
        }

        static bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return box_compare(a, b, 2);
        }
};

#endif
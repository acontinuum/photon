/*
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

std::vector<shared_ptr<triangle>> obj_to_triangles(std::string file_path, point position, shared_ptr<material> mat) {
    std::ifstream file(file_path);
    std::string current_line;
    std::string token;

    std::vector<point> vertices;
    std::vector<shared_ptr<triangle>> triangles;

    if(file.is_open()) {
        while(std::getline(file, current_line)) { // Loop through lines in the file
            char fchar = current_line[0]; // Get the first character

            if(fchar == 'v' && current_line[1] == ' ') { // Check if it is a vertex line
                std::string data =  current_line.substr(2, current_line.size() - 2); // Remove the first two charcters from the line - line definition and whitespace.
                std::istringstream stream(data); // Convert that line into a stream

                std::vector<std::string> numbers; // Create vector to store the x, y, z values.

                while (std::getline(stream, token, ' ' ) ) {
                    numbers.push_back(token); // Loop through the string and split it into the tokens.
                }
  
                vertices.push_back(point(stod(numbers[0]), stod(numbers[1]), stod(numbers[2])) + position); // Add the vertice to the vector with position offset.

            } else if(fchar == 'f') { // TODO - optimize by not checking for vertices after a face is found, since they are sequential in the file
                std::string data =  current_line.substr(2, current_line.size() - 2); // Remove the first two charcters from the line - line definition and whitespace.
                std::istringstream line_stream(data); // Convert that line into a stream

                std::vector<std::string> face_vertex_chunks; // Vector to store the blocks of vertex definitions
                std::vector<std::string> vertex_indices; // Vector to store the numbers in each block - we should have 9 numbers and 3 blocks

                while (std::getline(line_stream, token, ' ' ) ) {
                    face_vertex_chunks.push_back(token); // Split line into blocks
                }

                for(int i = 0; i < 3; i++) { 
                    std::istringstream chunk_stream(face_vertex_chunks[i]);
                    while (std::getline(chunk_stream, token, '/' ) ) {
                        vertex_indices.push_back(token); // Split blocks into vertex references
                    }
                }
                std::cout << "Adding face" << "at x: " << vertices[std::stoi(vertex_indices[0]) - 1] << "x: " << vertices[std::stoi(vertex_indices[3]) - 1] << "x: " << vertices[std::stoi(vertex_indices[6]) - 1] << std::endl;
                triangles.push_back(make_shared<triangle>(vertices[std::stoi(vertex_indices[0]) - 1], vertices[std::stoi(vertex_indices[3]) - 1], vertices[std::stoi(vertex_indices[6]) - 1], mat)); // Create a triangle with vertex's defined by the indices for position.
            }
        }

        file.close();
    }

    return triangles;
}

#endif
*/
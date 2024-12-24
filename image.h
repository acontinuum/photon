#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include"stb_image_write.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include "vector.h"

class image {
	public:
		int height;
		int width;
		uint8_t *data_ptr; // Pointer to a 2D array of color objects

	    // Constructor that takes width and height and allocates a 2D array of colors
	    image(int w, int h) {
	        height = h;
	        width = w;

	        // Dynamically allocate memory for the 2D array
	        data_ptr = new uint8_t[height * width * 3];  // Allocate an array of 'height' pointers

	        std::cout << "Image Initialized with size: " << width << "x" << height << std::endl;
	    }

	    // Destructor to deallocate memory
	    ~image() {
	        delete[] data_ptr;
	        std::cout << "Memory deallocated." << std::endl;
	    }

	   
	    void addPixel(int x, int y, color c) {
	        if (x >= 0 && x < width && y >= 0 && y < height) { // Check bounds, then write color value to uint_8t in list
	            data_ptr[(y * width + x) * 3] = static_cast<uint8_t>(c.x()* 255.99); // The int to write to is shifted by row, then x value, then r g and b of color.
	            data_ptr[(y * width + x) * 3 + 1] = static_cast<uint8_t>(c.y() * 255.99);
	            data_ptr[(y * width + x) * 3 + 2] = static_cast<uint8_t>(c.z() * 255.99);
	        } else {
	            std::cout << "ERROR - Pixel out of bounds." << std::endl;
	        }
	    }

		void saveImage(std::string filename) {
			if(stbi_write_png(filename.c_str(), width, height, 3, data_ptr, width * 3) != 0) {
				std::cout << "Image Written." << std::endl;
			} else {
				std::cout << "ERROR - Image Failed To Write.";
			}
		}
};

#endif

#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include "vector.h"

class image {
	public:
		int height;
		int width;
				
		image(int h, int w) {
			height = h; width = w;
			color data[width][height];
			std::cout << "Image Initialized.";
		}

		void saveImage(char const *filename, int comp) {
			if(stbi_write_png("") != 0) {
				std::cout << "Image Written."
			} else {
				std::cout << "ERROR - Image Failed To Write."
			}
		}

		void addPixel(int h, int w, color c) {
			//
		}
}

#endif

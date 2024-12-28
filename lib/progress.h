#ifndef PROGRESS_H
#define PROGRESS_H

#include <iostream>

class progress_bar {
	public:
		double progress;
		int barWidth = 70;

		void update(double p) {
			progress = p;
		    int pos = barWidth * progress;
		    std::cout << "[";
		    for (int i = 0; i < barWidth; ++i) {
		        if (i < pos) std::cout << "=";
		        else if (i == pos) std::cout << ">";
		        else std::cout << " ";
		    }
		    
		    std::cout << "] " << int(progress * 100.0) << " %\r";
		    std::cout.flush();
		}

		void end() {
			std::cout << std::endl;
		}
};

#endif

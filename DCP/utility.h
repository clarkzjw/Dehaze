#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <chrono>

class TicToc
{
private:
	typedef std::chrono::high_resolution_clock clock;
	typedef std::chrono::microseconds res;
	clock::time_point t1, t2;

public:
	void tic()
	{
		t1 = clock::now();
	}

	void toc()
	{
		t2 = clock::now();
		std::cout << "Elapsed time: "
			<< std::chrono::duration_cast<res>(t2 - t1).count() / 1e6
			<< " seconds." << std::endl;
	}
};

#endif
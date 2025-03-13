#pragma once
#include <vector>
#include <cmath>
using namespace std;

struct node {
	int i, j;
	int r, g, b;
};

class SeamCarver {
    vector<unsigned>& pixel;
    vector<double> pixelenergy;
    int w, h;
public:
	// create a seam carver object based on the given picture
    SeamCarver(vector<unsigned>& a, int width, int height);
    ~SeamCarver();

	double energy(int x, int y);
	void calenergy();

    void removeHorizontalSeam();
    void removeVerticalSeam();
};

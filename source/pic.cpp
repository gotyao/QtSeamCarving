#include "pic.h"
#include <QDebug>

SeamCarver::SeamCarver(vector<unsigned>& a, int width, int height)
    :pixel{a}, w{width}, h{height}
{ }

SeamCarver::~SeamCarver(){
    pixel.clear();
    pixelenergy.clear();
}

double SeamCarver::energy(int x, int y){
    int squarey;
    if (!(y % h)) {
        int R = (((pixel[(y + 1)*w + x]>>16) & 0xff) - ((pixel[(h - 1)*w + x]>>16) & 0xff))
               * (((pixel[(y + 1)*w + x]>>16) & 0xff) - ((pixel[(h - 1)*w + x]>>16) & 0xff));
        int G = (((pixel[(y + 1)*w + x]>>8) & 0xff) - ((pixel[(h - 1)*w + x]>>8) & 0xff))
               * (((pixel[(y + 1)*w + x]>>8) & 0xff) - ((pixel[(h - 1)*w + x]>>8) & 0xff));
        int B = ((pixel[(y + 1)*w + x] & 0xff) - (pixel[(h - 1)*w + x] & 0xff))
               * ((pixel[(y + 1)*w + x] & 0xff) - (pixel[(h - 1)*w + x] & 0xff));
        squarey = R + G + B;
	}
    else if (!((y+1) % h)) {
        int R = (((pixel[(y - 1)*w + x]>>16) & 0xff) - ((pixel[x]>>16) & 0xff))
                * (((pixel[(y - 1)*w + x]>>16) & 0xff) - ((pixel[x]>>16) & 0xff));
        int G = (((pixel[(y - 1)*w + x]>>8) & 0xff) - ((pixel[x]>>8) & 0xff))
                * (((pixel[(y - 1)*w + x]>>8) & 0xff) - ((pixel[x]>>8) & 0xff));
        int B = ((pixel[(y - 1)*w + x] & 0xff) - (pixel[x] & 0xff))
                * ((pixel[(y - 1)*w + x] & 0xff) - (pixel[x] & 0xff));
        squarey = R + G + B;
	}
	else {
        int R = (((pixel[(y - 1)*w + x]>>16) & 0xff) - ((pixel[(y + 1)*w + x]>>16) & 0xff))
                * (((pixel[(y - 1)*w + x]>>16) & 0xff) - ((pixel[(y + 1)*w + x]>>16) & 0xff));
        int G = (((pixel[(y - 1)*w + x]>>8) & 0xff) - ((pixel[(y + 1)*w + x]>>8) & 0xff))
                * (((pixel[(y - 1)*w + x]>>8) & 0xff) - ((pixel[(y + 1)*w + x]>>8) & 0xff));
        int B = ((pixel[(y - 1)*w + x] & 0xff) - (pixel[(y + 1)*w + x] & 0xff))
                * ((pixel[(y - 1)*w + x] & 0xff) - (pixel[(y + 1)*w + x] & 0xff));
        squarey = R + G + B;
	}

    int squarex;
    if (!(x % w)) {
        int R = (((pixel[y*w + x+1]>>16) & 0xff) - ((pixel[y*w + w-1]>>16) & 0xff))
                * (((pixel[y*w + x+1]>>16) & 0xff) - ((pixel[y*w + w-1]>>16) & 0xff));
        int G = (((pixel[y*w + x+1]>>8) & 0xff) - ((pixel[y*w + w-1]>>8) & 0xff))
                * (((pixel[y*w + x+1]>>8) & 0xff) - ((pixel[y*w + w-1]>>8) & 0xff));
        int B = ((pixel[y*w + x+1] & 0xff) - (pixel[y*w + w-1] & 0xff))
                * ((pixel[y*w + x+1] & 0xff) - (pixel[y*w + w-1] & 0xff));
        squarex = R + G + B;
	}
    else if (!((x+1) % w)) {
        int R = (((pixel[y*w]>>16) & 0xff) - ((pixel[y*w + x-1]>>16) & 0xff))
                * (((pixel[y*w]>>16) & 0xff) - ((pixel[y*w + x-1]>>16) & 0xff));
        int G = (((pixel[y*w]>>8) & 0xff) - ((pixel[y*w + x-1]>>8) & 0xff))
                * (((pixel[y*w]>>8) & 0xff) - ((pixel[y*w + x-1]>>8) & 0xff));
        int B = ((pixel[y*w] & 0xff) - (pixel[y*w + x-1] & 0xff))
                * ((pixel[y*w] & 0xff) - (pixel[y*w + x-1] & 0xff));
        squarex = R + G + B;
	}
	else {
        int R = (((pixel[y*w + x+1]>>16) & 0xff) - ((pixel[y*w + x-1]>>16) & 0xff))
                * (((pixel[y*w + x+1]>>16) & 0xff) - ((pixel[y*w + x-1]>>16) & 0xff));
        int G = (((pixel[y*w + x+1]>>8) & 0xff) - ((pixel[y*w + x-1]>>8) & 0xff))
                * (((pixel[y*w + x+1]>>8) & 0xff) - ((pixel[y*w + x-1]>>8) & 0xff));
        int B = ((pixel[y*w + x+1] & 0xff) - (pixel[y*w + x-1] & 0xff))
                * ((pixel[y*w + x+1] & 0xff) - (pixel[y*w + x-1] & 0xff));
        squarex = R + G + B;
	}

	return sqrt(squarex + squarey);
}

void SeamCarver::calenergy(){
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pixelenergy.push_back(energy(j, i));
		}
    }
}

void SeamCarver::removeHorizontalSeam(){
    pixelenergy.clear();
    calenergy();
    //dp
    //for horizonal, make turn in temp
    vector<double> temp;
    for(int j = 0; j < w; j++){
        if(!j){
            for(int i = 0; i < h; i++){
                temp.push_back(pixelenergy[i*w + j]);
            }
        }
        else{
            for(int i = 0; i < h; i++){
                temp.push_back(pixelenergy[i*w + j] + temp[(j-1)*h + i]);

                if(i % h && temp[(j-1)*h + i-1] < temp[(j-1)*h + i]){
                    temp[j*h + i] = temp[(j-1)*h + i-1] + pixelenergy[i*w + j];
                }
                if((i+1) % h && temp[(j-1)*h + i+1] < temp[(j-1)*h + i]){
                    temp[j*h + i] = temp[(j-1)*h + i+1] + pixelenergy[i*w + j];
                }
            }
        }
    }

    int min{0};
    for(int i = 1; i < h; i++){
        if(temp[(w-1)*h + min] > temp[(w-1)*h + i]){
            min = i;
        }
    }

    //find path
    vector<int> path;
    int f = (w-1)*h + min;
    int x, y;
    x = f/h;
    y = f%h;
    path.push_back(y*w + x);
    while(f >= h){//turn back
        double min{temp[f-h]};
        int m = f;
        if(f % h && min > temp[f-h-1]){
            min = temp[f-h-1];
            m = f-1;
        }
        if((f+1) % h && min > temp[f-h+1]){
            m = f+1;
        }

        f = m - h;
        x = f/h;
        y = f%h;
        path.push_back(y*w + x);
    }

    //remove
    for(auto e:path){
        int i = e;
        while(i >= w){
            swap(pixel[i], pixel[i-w]);
            i -= w;
        }
    }
    pixel.erase(pixel.begin(), pixel.begin() + w);

    h--;
    pixelenergy.clear();
}

void SeamCarver::removeVerticalSeam(){
    calenergy();
    //dp
    vector<double> temp;
    for(int i = 0; i < h; i++){
        if(!i){
            for(int j = 0; j < w; j++){
                temp.push_back(pixelenergy[i*w + j]);
            }
        }
        else{
            for(int j = 0; j < w; j++){
                temp.push_back(pixelenergy[i*w + j] + temp[(i-1)*w + j]);
                if(j % w && temp[(i-1)*w + j-1] < temp[(i-1)*w + j]){
                    temp[i*w + j] = temp[(i-1)*w + j-1] + pixelenergy[i*w + j];
                }
                if((j+1) % w && temp[(i-1)*w + j+1] < temp[(i-1)*w + j]){
                    temp[i*w + j] = temp[(i-1)*w + j+1] + pixelenergy[i*w + j];
                }
            }
        }
    }

    int min{0};
    for(int j = 1; j < w; j++){
        if(temp[(h-1)*w + min] > temp[(h-1)*w + j]){
            min = j;
        }
    }

    //find path
    vector<int> path;
    int f = (h-1)*w + min;
    path.push_back(f);
    while(f >= w){
        double min{temp[f-w]};
        int m = f;
        if(f % w && min > temp[f-w-1]){
            min = temp[f-w-1];
            m = f-1;
        }
        if((f+1) % w && min > temp[f-w+1]){
            m = f+1;
        }

        f = m - w;
        path.push_back(f);
    }

    //remove
    for(auto e:path){
        pixel.erase(pixel.begin() + e);
    }

    w--;
    pixelenergy.clear();
}

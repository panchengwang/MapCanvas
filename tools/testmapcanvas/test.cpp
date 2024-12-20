#include <iostream>
#include <stdlib.h>
#include "../../cpp/Canvas.h"
#include <sys/time.h>


int main(int argc, char** argv){

    struct timeval start;
    gettimeofday(&start, NULL);

    MapCanvas canvas;
    canvas.setDotsPerMM(600/25.4);
    canvas.setCenter(0,0);
    canvas.setWidth(360);
    canvas.setHeight(180);
    canvas.begin();
    for(int i=0; i<100; i++){
        canvas.draw("POINT(0 0)");
    }
    canvas.draw("LINESTRING(-180 0, 180 0)");
    canvas.draw("LINESTRING(0 -90, 0 90)");
    canvas.end();
    // canvas.saveToFile("a.png");

    // size_t len;
    // unsigned char* data = canvas.imageData(len);
    // delete [] data;

    struct timeval end;
    gettimeofday(&end, NULL);

    std::cout << "time used: " << ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000.0 << std::endl;
    return EXIT_SUCCESS;
}

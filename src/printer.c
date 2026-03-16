#include "printer.h"

//https://www.geeksforgeeks.org/dsa/bresenhams-line-generation-algorithm/
//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void InsertBrensenhamLine(Vec2* pPointI, Vec2* pPointF, ImageBuffer* image){
    //Points should be in screen domain from x: [0, width], y: [0, height]
    int cols = image->cols;
    int xf = pPointF->vec[0]; int xi = pPointI->vec[0];
    int yf = pPointF->vec[1]; int yi = pPointI->vec[1];

    int deltaX = abs(xf-xi);
    int dirX = (xf > xi) - (xf < xi);
    int deltaY = -abs(yf-yi);
    int dirY = (yf > yi) - (yf < yi);

    int err = deltaX + deltaY;

    int i = 0;
    do {
        image->pImage[cols*(yi+i*dirY) + (xi+i*dirX)] = '#';
        i++;
    } while (i < abs(err)*(((bool)deltaX) ^ ((bool)deltaY)));

    while (deltaX && deltaY){
        image->pImage[cols*yi + xi] = '#';
        int e2 = 2 * err;
        if (e2 >= deltaY){
            if (xi == xf) break;
            err += deltaY;
            xi += dirX; 
        }
        if (e2 < deltaX){
            if (yi == yf) break;
            err += deltaX;
            yi += dirY;
        }
    }
}

void ClearImageBuffer(ImageBuffer* buffer){
    //Fills image buffer with spaces (over rides any characters present)
    for (int i=0; i<(buffer->cols*buffer->rows); i++){
        buffer->pImage[i] = ' ';
    }
}
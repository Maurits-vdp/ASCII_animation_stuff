#include "printer.h"
#ifndef SQUISH_Y
    float squish_factor = 0.5;
#else
    float squish_factor = 1;
#endif


void DrawVertices(ImageBuffer* image, Vec4* vertices, int* indices){
    int numIndices = 6; //Will this work?
    //Iterate per triangle to draw
    for (int triaIndex=0; triaIndex<(numIndices/3); triaIndex++){
        //probably a better way to do this
        int triangleIndices[3] = {indices[triaIndex*3], indices[triaIndex*3 + 1], indices[triaIndex*3 + 2]}; 
        
        for (int i=0; i<3; i++){
            int iNext = (i+1)%3;
            int index = triangleIndices[i];
            int indexNext = triangleIndices[iNext];
            
            Vec2 pointStart = {
                .vec = {vertices[index].vec[0], squish_factor*vertices[index].vec[1]} //multiplying by 0.5 to stop squishing
            };
            Vec2 pointFinish = {
                .vec = {vertices[indexNext].vec[0], squish_factor*vertices[indexNext].vec[1]}
            };

            InsertBrensenhamLine(&pointStart, &pointFinish, image);
        }
    }
}

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
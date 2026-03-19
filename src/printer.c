#include "printer.h"

void PrintImage(ImageBuffer* pImageBuf){
    printf("\033[H");
    char* pBuffer = calloc((pImageBuf->cols+1)*pImageBuf->rows + 1, sizeof(char));
    char* pString = pBuffer;
    for (int row=pImageBuf->rows-1; row >= 0; row--){ // reversed order to flip Y axis
        for (int col=0; col<pImageBuf->cols; col++){
            *pString = pImageBuf->pImage[row*pImageBuf->cols + col];
            pString++;
        }
        *pString = '\n';
        pString++;
    }
    *pString = '\0';
    printf("%s", pBuffer);
    free(pBuffer);
}

void DrawVertices(ImageBuffer* image, Vec4* vertices, int* indices, int numIndices){
    //Iterate per triangle to draw
    for (int triaIndex=0; triaIndex<(numIndices/3); triaIndex++){
        //probably a better way to do this
        int triangleIndices[3] = {indices[triaIndex*3], indices[triaIndex*3 + 1], indices[triaIndex*3 + 2]}; 
        
        for (int i=0; i<(2+DIAGS_ENABLED); i++){
            int iNext = (i+1)%3;
            int index = triangleIndices[i];
            int indexNext = triangleIndices[iNext];
            
            Vec2 pointStart = {
                .vec = {vertices[index].vec[0], SQUISH_FACTOR*vertices[index].vec[1]} //multiplying by 0.5 to stop squishing
            };
            Vec2 pointFinish = {
                .vec = {vertices[indexNext].vec[0], SQUISH_FACTOR*vertices[indexNext].vec[1]}
            };

            //printf("Drawing indices: (%i, %i), for triangle number: %i", indices[triaIndex*3], indices[triaIndex*3+1], triaIndex);
            //Sleep(2000);
            InsertBrensenhamLine(&pointStart, &pointFinish, image, '#');
            //PrintImage(image);
        }
    }
}

//https://www.geeksforgeeks.org/dsa/bresenhams-line-generation-algorithm/
//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void InsertBrensenhamLine(Vec2* pPointI, Vec2* pPointF, ImageBuffer* image, char character){
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
        image->pImage[cols*(yi+i*dirY) + (xi+i*dirX)] = character;
        i++;
    } while (i < abs(err)*(((bool)deltaX) ^ ((bool)deltaY)));

    while (deltaX && deltaY){
        image->pImage[cols*yi + xi] = character;
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
#include <Windows.h>
#include <time.h>
#include <math.h>
#include "printer.h"
#define PI 3.1415926535

void PrintImage(ImageBuffer* pImageBuf){
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

int main(){
    ImageBuffer image = {
        .cols = 30,
        .rows = 30,
        .pImage = calloc(image.cols*image.rows, sizeof(char)),
    };

    int radius = 10;
    Vec2 initPoint = {
        .vec = {0,15 - radius}
    };
    Vec2 radPoint = {
        .vec = {0, 15 + radius}
    };

    ClearImageBuffer(&image);
    InsertBrensenhamLine(&initPoint, &radPoint, &image);
    PrintImage(&image);

    double tElapsed = 0;
    double dt;
    double fOmega = 4*(2*PI/10);
    double theta = 0;

    clock_t t0 = clock();
    while (tElapsed < 10){
        clock_t tNow = clock();
        dt = (double) (tNow - t0)/(CLOCKS_PER_SEC);
        theta += fOmega * dt;

        radPoint.vec[0] = 15 + (int) radius*cos(theta);
        radPoint.vec[1] = 15 + (int) radius*sin(theta);

        initPoint.vec[0] = 15 - (int) radius*cos(theta);
        initPoint.vec[1] = 15 - (int) radius*sin(theta);

        ClearImageBuffer(&image);
        InsertBrensenhamLine(&initPoint, &radPoint, &image);
        PrintImage(&image);

        tElapsed += dt;
        t0 = tNow;
    }

    Sleep(500);
    return 0;
}

    /*
   Vec2 point1 = {
    .vec = {0, 0}
   };
   Vec2 point2 = {
    .vec = {20, 20}
   };
   Vec2 pointA = {
    .vec = {20,0}
   };
   Vec2 pointB = {
    .vec = {0,20}
   };
   Vec2 pH1 = {
    .vec = {5, 10}
   };
   Vec2 pH2 = {
    .vec = {5, 5}
   };
   */
#include <Windows.h>
#include <time.h>
#include <math.h>
#include "printer.h"
#define PI 3.1415926535
#define SQUISH_Y
#define WIDTH 50
#define HEIGHT 25 //Must be half of width for a square aspect ratio if SQUISH_Y is defined

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
        .cols = WIDTH,
        .rows = HEIGHT,
        .pImage = calloc(image.cols*image.rows, sizeof(char)),
    };

    //Vertices defined from [-0.5, 0.5] for all axes
    size_t nVertices;
    Vec4 vertList[4] = {
        {0.5, 0.5, -0.5, 0},
        {0.5, -0.5, -0.5 , 0},
        {-0.5, -0.5, -0.5 , 0},
        {-0.5, 0.5, -0.5, 0}
    };

    int indices[6] = {0, 1, 3, 3, 2, 1}; //6 move instructions,
    //Optimise by removing duplicate instructions? (see: .. , 1, 3, 3, ...) 
    //Change to Vec2* array to store pointers to vertices?    

    /* All this is commented for future reference
    Mat4x4 model; CreateMat4x4(&model, 1); Model matrix is for rotating the model
    Mat4x4 view; CreateMat4x4(&view, 1);
    Vec4 transVec = {0,0,0,0};
    Translate(&view, &transVec);
    //V_{clip} = M_{projection} * M_{view} * M_{model} * V_{Local}
    //Translate and scale each vertex into the screen domain
    //M_{model}
    */

    double tElapsed = 0;
    double dt;

    double angfreq = 2*PI/(2);
    double theta = 0;
    double radius = 20;

    Vec4 testList[4] = {
        {20 + radius*cos(theta), 20 + radius*sin(theta)},
        {20 + radius*cos(theta+PI/2), 20 + radius*sin(theta+PI/2)},
        {20 + radius*cos(theta+PI), 20 + radius*sin(theta+PI)},
        {20 + radius*cos(theta+3*PI/2), 20 + radius*sin(theta+3*PI/2)}
    };
    int testIndices[6] = {0, 3, 2, 2, 1, 0};

    ClearImageBuffer(&image);
    DrawVertices(&image, testList, testIndices);
    PrintImage(&image);

    clock_t t0 = clock();
    while (tElapsed < 20){
        clock_t tNow = clock();
        dt = (double) (tNow - t0)/(CLOCKS_PER_SEC);

        theta += angfreq *dt;

        //Could be improved but I can't be asked right now
        testList[0].vec[0] = 20 + radius*cos(theta);
        testList[0].vec[1] = 20 + radius*sin(theta);

        testList[1].vec[0] = 20 + radius*cos(theta+PI/2);
        testList[1].vec[1] = 20 + radius*sin(theta+PI/2);

        testList[2].vec[0] = 20 + radius*cos(theta+PI);
        testList[2].vec[1] = 20 + radius*sin(theta+PI);

        testList[3].vec[0] = 20 + radius*cos(theta+3*PI/2);
        testList[3].vec[1] = 20 + radius*sin(theta+3*PI/2);

        ClearImageBuffer(&image);
        DrawVertices(&image, testList, testIndices);
        PrintImage(&image);
        
        tElapsed += dt;
        t0 = tNow;
    }
    return 0;
}
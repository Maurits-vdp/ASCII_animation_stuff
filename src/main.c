#include <Windows.h>
#include <time.h>
#include <math.h>
#include "printer.h"

#define PI 3.1415926535
#define SQUISH_Y
#define WIDTH 200
#define HEIGHT 100 //Must be half of width for a square aspect ratio if SQUISH_Y is defined
#define DegToRad(theta) theta*(PI)/180
//Note that if the width and height defined don't fit the terminal, it'll mess with the visuals. Zoom out manually in the terminal to fix this?

void InitScreen(){
    printf("\x1B[?25l");
}
void KillScreen(){
    printf("\x1B[?25h");
}

int main(){
    ImageBuffer image = {
        .cols = WIDTH,
        .rows = HEIGHT,
        .pImage = calloc(image.cols*image.rows, sizeof(char)),
    };

    //Vertices defined from [-1, 1] for all axes although setting to [-0.5, 0.5 is what I like]
    size_t nVertices;
       Vec4 squareList[4] = {
        {0.5, 0, 0.5, 1.0},
        {0.5, 0, -0.5 , 1.0},
        {-0.5, 0, -0.5 , 1.0},
        {-0.5, 0, 0.5, 1.0}
    };

    nVertices = sizeof(squareList)/sizeof(Vec4);

    int indices[6] = {0, 1, 3, 3, 2, 1}; //6 move instructions,
    //Optimise by removing duplicate instructions? (see: .. , 1, 3, 3, ...) 
    //Change to Vec2* array to store pointers to vertices?    

    double angfreq = 2*PI/(2);
    double theta = 0;
    double radius = 1;

    //Camera stuff (view)
    Vec3 viewingPosition = {.vec = {radius,0.5,radius}};
    Vec3 targetPosition = {.vec = {0,0,0}};
    Vec3 upVector = {.vec = {0,1,0}};
    Mat4x4 viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector); //Is there an issue with this again???

    //Projection matrix stuff
    Mat4x4 projection; CreateMat4x4(&projection, 1.0f);
    VFOVPerspectiveProj(&projection, DegToRad(90.0f), ((float)WIDTH/(float)HEIGHT), 0.1f, 100.0f);

    //translation of model, (currently not doing that)
    Vec4 transVec = {.vec = {0,0,0,0}};
    Mat4x4 model; CreateMat4x4(&model, 1.0f);
    CreateTranslationMatrix(&model, &transVec);

    //Copying square stuff into the vertex buffer
    Vec4 vertList[4];
    memcpy(vertList, squareList, sizeof(squareList));
    for (int i = 0; i<nVertices; i++){ //Make a function for this? LocalToScreen? Split up into Local2Clip & Clip2View?
        //V_{clip} = M_{projection} * M_{view} * M_{model} * V_{Local}
        //M_{model} = translation * rotation (rotation first)
        //M_{view} = 

        VecMatrixProduct(&model, &vertList[i]);
        VecMatrixProduct(&viewMatrix, &vertList[i]);
        VecMatrixProduct(&projection, &vertList[i]);
        ConvertClipToNDC(&vertList[i]);
        ViewportTransform(&vertList[i], WIDTH, HEIGHT);
    }

    double tElapsed = 0;
    double dt;

    InitScreen();
    ClearImageBuffer(&image);
    DrawVertices(&image, vertList, indices);
    PrintImage(&image);
    
    clock_t t0 = clock();
    while (tElapsed < 20){
        clock_t tNow = clock();
        dt = (double) (tNow - t0)/(CLOCKS_PER_SEC);

        memcpy(vertList, squareList, sizeof(squareList));
        theta += angfreq*dt;
        viewingPosition.vec[0] = radius*cos(theta);
        viewingPosition.vec[2] = radius*sin(theta);
        viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector);

        for (int i = 0; i<nVertices; i++){
            VecMatrixProduct(&model, &vertList[i]);
            VecMatrixProduct(&viewMatrix, &vertList[i]);
            VecMatrixProduct(&projection, &vertList[i]);
            ConvertClipToNDC(&vertList[i]);
            ViewportTransform(&vertList[i], WIDTH, HEIGHT);
        }
        
        ClearImageBuffer(&image);
        DrawVertices(&image, vertList, indices);
        PrintImage(&image);
        
        tElapsed += dt;
        t0 = tNow;
    }

    KillScreen();
    return 0;
}
#include <Windows.h>
#include <time.h>
#include <math.h>
#include "printer.h"
#include "basic_shapes.h"

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

void DoMathOnVertLis(Mat4x4* projection, Mat4x4* viewMatrix, Mat4x4* model, Vec4* vertList, int nVertices){
    for (int i = 0; i<nVertices; i++){ //Make a function for this? LocalToScreen? Split up into Local2Clip & Clip2View?
        //V_{clip} = M_{projection} * M_{view} * M_{model} * V_{Local}
        //M_{model} = translation * rotation (rotation first)
        //M_{view} = 

        VecMatrixProduct(model, &vertList[i]);
        VecMatrixProduct(viewMatrix, &vertList[i]);
        VecMatrixProduct(projection, &vertList[i]);
        ConvertClipToNDC(&vertList[i]);
        ViewportTransform(&vertList[i], WIDTH, HEIGHT);
    }
}

int main(){
    ImageBuffer image = {
        .cols = WIDTH,
        .rows = HEIGHT,
        .pImage = calloc(image.cols*image.rows, sizeof(char)),
    };

    //Vertices defined from [-1, 1] for all axes although setting to [-0.5, 0.5 is what I like]
    Shape cubeCopy;
    CopyReferenceShape(&cubeCopy, CUBE);

    /*
    int numVertices = 8;
    Vec4 cubeList[] = {
        {0.5, 0.5, 0.5, 1.0},
        {0.5, 0.5, -0.5 , 1.0}, 
        {0.5, -0.5, 0.5 , 1.0},
        {0.5, -0.5, -0.5, 1.0},

        {-0.5, 0.5, 0.5, 1.0},
        {-0.5, 0.5, -0.5 , 1.0}, 
        {-0.5, -0.5, 0.5 , 1.0},
        {-0.5, -0.5, -0.5, 1.0}
    };

    int indices[] = {
        0, 1, 3, 3, 2, 0, //xmax plane
        4, 5, 7, 7, 6, 4, //xmin plane

        0, 4, 5, 5, 1, 0, //ymax plane
        2, 6, 7, 7, 3, 2, //ymin plane

        0, 4, 6, 6, 2, 0, //zmax plane
        1, 5, 7, 7, 3, 1, //zmin plane

    };
    */

    double angfreq = 2*PI/(2);
    double theta = 0;
    double radius = 1.2;

    //Projection matrix stuff
    Mat4x4 projection; CreateMat4x4(&projection, 1.0f);
    VFOVPerspectiveProj(&projection, DegToRad(90.0f), ((float)WIDTH/(float)HEIGHT), 0.1f, 100.0f);

    //translation of model, (currently not doing that)
    Vec4 transVec = {.vec = {0,0.5,0,0}};
    Mat4x4 model; CreateMat4x4(&model, 1.0f);
    CreateTranslationMatrix(&model, &transVec);

    //Camera stuff (view)
    Vec3 viewingPosition = {.vec = {radius,radius,radius}};
    Vec3 targetPosition = {.vec = {0,0.5,0}};
    Vec3 upVector = {.vec = {0,1,0}};
    Mat4x4 viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector); //Is there an issue with this again???

    //Copying cube stuff into the vertex buffer
    int numVertices = cubeCopy.mNumVertices;
    Vec4* vertList = malloc(numVertices*sizeof(cubeCopy.mVertices[0]));
    memcpy(vertList, cubeCopy.mVertices, numVertices*sizeof(cubeCopy.mVertices[0]));
    DoMathOnVertLis(&projection, &viewMatrix, &model, vertList, numVertices);

    double tElapsed = 0;
    double dt;

    int numIndices = cubeCopy.mNumIndices;
    InitScreen();
    ClearImageBuffer(&image);
    DrawVertices(&image, vertList, cubeCopy.mIndices, numIndices);
    PrintImage(&image);
    
    float tEnd = 20;
    clock_t t0 = clock();
    while (tElapsed <= tEnd){
        clock_t tNow = clock();
        dt = (double) (tNow - t0)/(CLOCKS_PER_SEC);

        memcpy(vertList, cubeCopy.mVertices, numVertices*sizeof(cubeCopy.mVertices[0]));
    
        theta += angfreq*dt;
        viewingPosition.vec[0] = radius*cos(theta);
        viewingPosition.vec[2] = radius*sin(theta);
        viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector);

        DoMathOnVertLis(&projection, &viewMatrix, &model, vertList, numVertices);
        
        ClearImageBuffer(&image);
        DrawVertices(&image, vertList, cubeCopy.mIndices, numIndices);
        PrintImage(&image);
        
        tElapsed += dt;
        t0 = tNow;
    }
    
    Sleep(3000);
    free(vertList);

    KillScreen();
    return 0;
}
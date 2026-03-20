#include "debug_tests.h"

void InitScreen(){
    printf("\x1B[?25l");
}
void KillScreen(){
    printf("\x1B[?25h");
}

void DoMathOnVertList(Mat4x4* projection, Mat4x4* viewMatrix, Mat4x4* model, Vec4* vertList, int nVertices){
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

//Renders testing shape under controlled conditions
void PreviewShape(enum ReferenceShapes shapeID){
    ImageBuffer image = {
        .cols = WIDTH,
        .rows = HEIGHT,
        .pImage = calloc(image.cols*image.rows, sizeof(char)),
    };

    //Vertices defined from [-1, 1] for all axes although setting to [-0.5, 0.5 is what I like]
    Shape shapeCopy;
    shapeCopy.mVertices = malloc(GetShapeVertMallocSize(shapeID));
    shapeCopy.mIndices = malloc(GetShapeIndexMallocSize(shapeID));
    CopyReferenceShape(&shapeCopy, shapeID);

    double angfreq = 2*PI/(2);
    double theta = 0;
    double radius = 1.2;

    //Projection matrix stuff
    Mat4x4 projection; CreateMat4x4(&projection, 1.0f);
    VFOVPerspectiveProj(&projection, DegToRad(90.0f), ((float)WIDTH/(float)HEIGHT), 0.1f, 100.0f);

    //translation of model, (currently not doing that)
    //Vec4 transVec = {.vec = {0,0,0,0}};
    Mat4x4 model; CreateMat4x4(&model, 1.0f);
    //CreateTranslationMatrix(&model, &transVec);

    //Camera stuff (view)
    Vec3 viewingPosition = {.vec = {radius, 0.25 ,radius}};
    Vec3 targetPosition = {.vec = {0, 0, 0}};
    Vec3 upVector = {.vec = {0,1,0}};
    Mat4x4 viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector); //Is there an issue with this again???

    //Copying cube stuff into the vertex buffer
    int numVertices = shapeCopy.mNumVertices;
    Vec4* vertList = malloc(numVertices*sizeof(shapeCopy.mVertices[0]));
    memcpy(vertList, shapeCopy.mVertices, numVertices*sizeof(shapeCopy.mVertices[0]));
    DoMathOnVertList(&projection, &viewMatrix, &model, vertList, numVertices);

    double tElapsed = 0;
    double dt;

    int numIndices = shapeCopy.mNumIndices;
    InitScreen();
    ClearImageBuffer(&image);
    DrawVertices(&image, vertList, shapeCopy.mIndices, numIndices);
    PrintImage(&image);
    
    clock_t t0 = clock();
    while (tElapsed <= 4){
        clock_t tNow = clock();
        dt = (double) (tNow - t0)/(CLOCKS_PER_SEC);

        memcpy(vertList, shapeCopy.mVertices, numVertices*sizeof(shapeCopy.mVertices[0]));
    
        theta += angfreq*dt;
        viewingPosition.vec[0] = radius*cos(theta);
        viewingPosition.vec[2] = radius*sin(theta);
        viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector);

        DoMathOnVertList(&projection, &viewMatrix, &model, vertList, numVertices);
        
        ClearImageBuffer(&image);
        DrawVertices(&image, vertList, shapeCopy.mIndices, numIndices);
        PrintImage(&image);
        
        tElapsed += dt;
        t0 = tNow;
    }

    tElapsed = 0;
    viewingPosition.vec[0] = radius;
    viewingPosition.vec[1] = radius;
    viewingPosition.vec[2] = 1.2;

    while (tElapsed <= 4){
        clock_t tNow = clock();
        dt = (double) (tNow - t0)/(CLOCKS_PER_SEC);

        memcpy(vertList, shapeCopy.mVertices, numVertices*sizeof(shapeCopy.mVertices[0]));
    
        theta += angfreq*dt;
        viewingPosition.vec[0] = radius*cos(theta);
        viewingPosition.vec[1] = radius*sin(theta);

        viewMatrix = LookAtMatrix(&viewingPosition, &targetPosition, &upVector);

        DoMathOnVertList(&projection, &viewMatrix, &model, vertList, numVertices);
        
        ClearImageBuffer(&image);
        DrawVertices(&image, vertList, shapeCopy.mIndices, numIndices);
        PrintImage(&image);
        
        tElapsed += dt;
        t0 = tNow;
    }
    free(vertList);
    free(image.pImage);
    free(shapeCopy.mIndices);
    free(shapeCopy.mVertices);

    KillScreen();
    return;
}

//gcc -Isrc debug/debug_tests.c src/printer.c src/basic_shapes.c src/geometry.c -o debug/debug_test
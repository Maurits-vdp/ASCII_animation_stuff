#include "basic_shapes.h"
#define NUM_SHAPES 3
#define ArrayLength(array, dataType) (sizeof(array)/sizeof(dataType))

//Plane or square centred at origin, flat relative to y axis
static Vec4 squareVertices[] = {
    {0.5, 0, 0.5, 1.0},
    {0.5, 0, -0.5 , 1.0},
    {-0.5, 0, -0.5 , 1.0},
    {-0.5, 0, 0.5, 1.0}
};
static int squareIndices[] = {0, 1, 3, 3, 2, 1};
Shape plane = {
    .mNumVertices = ArrayLength(squareVertices, Vec4),
    .mVertices =squareVertices,
    .mNumIndices =ArrayLength(squareIndices, int),
    .mIndices = squareIndices,
};

//Equilateral triangle centred at origin, flat relative to y axis
//Tip is at the z+ axis
static Vec4 eqTriangleVertices[] = {
    {0.0, 0, 0.5, 1.0},
    {0.5, 0, -0.5 , 1.0},
    {-0.5, 0, -0.5 , 1.0},
};
static int eqTriangleIndices[] = {0, 1, 2};
Shape equilTriangle = {
    .mNumVertices = ArrayLength(eqTriangleVertices, Vec4),
    .mVertices = eqTriangleVertices,
    .mNumIndices =ArrayLength(eqTriangleIndices, int),
    .mIndices = eqTriangleIndices,
};

//Cube centred at origin
static Vec4 cubeVertices[] = {
    {0.5, 0.5, 0.5, 1.0},
    {0.5, 0.5, -0.5 , 1.0}, 
    {0.5, -0.5, 0.5 , 1.0},
    {0.5, -0.5, -0.5, 1.0},

    {-0.5, 0.5, 0.5, 1.0},
    {-0.5, 0.5, -0.5 , 1.0}, 
    {-0.5, -0.5, 0.5 , 1.0},
    {-0.5, -0.5, -0.5, 1.0}
};
static int cubeIndices[] = {
    0, 1, 3, 3, 2, 0, //xmax plane
    4, 5, 7, 7, 6, 4, //xmin plane

    0, 4, 5, 5, 1, 0, //ymax plane
    2, 6, 7, 7, 3, 2, //ymin plane

    0, 4, 6, 6, 2, 0, //zmax plane
    1, 5, 7, 7, 3, 1, //zmin plane

};
Shape cube = {
    .mNumVertices = ArrayLength(cubeVertices, Vec4),
    .mVertices = cubeVertices,
    .mNumIndices =ArrayLength(cubeIndices, int),
    .mIndices = cubeIndices,
};
//Pyramid centred at origin, ape

static Shape* shapesArray[NUM_SHAPES] = {
    &plane, &equilTriangle, &cube
};

void CopyReferenceShape(Shape* shapeBuf, enum ReferenceShapes shapeID){
    const Shape* refShape = shapesArray[shapeID];
    printf("Num reference vertices: %i\n", refShape->mNumVertices);
    shapeBuf->mNumVertices = refShape->mNumVertices;
    printf("Number assigned vertices: %i\n", shapeBuf->mNumVertices);
    shapeBuf->mNumIndices = refShape->mNumIndices;

    shapeBuf->mVertices = malloc(refShape->mNumVertices*sizeof(refShape->mVertices[0]));
    memcpy(shapeBuf->mVertices, refShape->mVertices, refShape->mNumVertices*sizeof(refShape->mVertices[0]));

    shapeBuf->mIndices = malloc(refShape->mNumIndices*sizeof(refShape->mIndices[0]));
    memcpy(shapeBuf->mIndices, refShape->mIndices, refShape->mNumIndices*sizeof(refShape->mIndices[0]));
    return;
}
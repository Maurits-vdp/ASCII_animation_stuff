#include "geometry.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef BASIC_SHAPES_H
#define BASIC_SHAPES_H

extern enum ReferenceShapes {
    PLANE,
    EQUIL_TRIANGLE,
    CUBE
} ReferenceShapes;

//Vertices defined from [-1, 1] for all axes although setting to [-0.5, 0.5 is what I like]
typedef struct {
    int mNumVertices;
    Vec4* mVertices; //Use Vec3 and then add 1.0f to last coords?
    int mNumIndices;
    int* mIndices;
} Shape;

void CopyReferenceShape(Shape* shapeBuf, enum ReferenceShapes shapeID);

//Note on indices
//Optimise by removing duplicate instructions? (see: .. , 1, 3, 3, ...) 
//Change to Vec2* array to store pointers to vertices?    

//Remember to free the shapeBuf->mVertices and ->mIndices after use!

#endif
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int cols;
    int rows;
    char* pImage;
} ImageBuffer;

void InsertBrensenhamLine(Vec2* pPointI, Vec2* pPointF, ImageBuffer* image);
void ClearImageBuffer(ImageBuffer* buffer);